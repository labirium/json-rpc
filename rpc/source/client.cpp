
#include <rpc/include/precompiled.h>
#include <rpc/include/client.h>
#include <assert.h>

using namespace rpc;
using namespace rpc::client;


void Connect::StartConnect(ConnectPtr connect, const boost::asio::ip::tcp::resolver::iterator& endpoint_iter)
{
	if (endpoint_iter == boost::asio::ip::tcp::resolver::iterator()){
		// There are no more endpoints to try. Shut down the client.
		Stop();
		return;
	}

	std::cout << "Trying " << endpoint_iter->endpoint() << "...\n";

	// Set a deadline for the connect operation.
	deadline_.expires_from_now(boost::posix_time::seconds(60));

	// Start the asynchronous connect operation.
	socket_.lowest_layer().async_connect(endpoint_iter->endpoint(), boost::bind(&Connect::HandleConnect, this, connect, _1, endpoint_iter));
}

void Connect::HandleHandshake(ConnectPtr connect, const boost::system::error_code& error)
{
	if (error)
	{
		std::cout << "Handshake failed: " << error.message() << "\n";
		// There are no more endpoints to try. Shut down the client.
		Stop();
		return;
	}

	// Start the input actor.
	StartRead(connect);

	// Start the heartbeat actor.
	StartWrite(connect);
}

void Connect::HandleConnect(ConnectPtr connect, const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator endpoint_iter)
{
	if (stopped_){
		return;
	}

	if (!socket_.lowest_layer().is_open())
	{
		std::cout << "Connect timed out\n";

		// Try the next available endpoint.
		StartConnect(connect, ++endpoint_iter);
	}
	else if (ec)
	{
		std::cout << "Connect error: " << ec.message() << "\n";

		// We need to close the socket used in the previous connection attempt before starting a new one.
		socket_.lowest_layer().close();

		// Try the next available endpoint.
		StartConnect(connect, ++endpoint_iter);
	}
	else
	{
		std::cout << "Connected to " << endpoint_iter->endpoint() << "\n";

		socket_.async_handshake
		(
			boost::asio::ssl::stream_base::client,
			boost::bind(&Connect::HandleHandshake, this, connect, boost::asio::placeholders::error)
		);

	}
}

void Connect::StartRead(ConnectPtr connect)
{
	// Set a deadline for the read operation.
	deadline_.expires_from_now(boost::posix_time::seconds(30));

	memset(buffer_.data(), 0x00, buffer_.size());

	// Start an asynchronous operation to read a newline-delimited message.
	socket_.async_read_some(boost::asio::buffer(buffer_), boost::bind(&Connect::HandleRead, this, connect, _1, _2, BlobData()));
}

void Connect::HandleRead(ConnectPtr connect, const boost::system::error_code& errorCode, std::size_t transferred, const BlobData& blobData)
{
	if (errorCode){
		std::wcout << L"Invalid connect to server." << std::endl;
		Stop();
		return;
	}
	
	if (0 != blobData.size())
	{
		ArrayBuffer bufferParse;
		memcpy(bufferParse.data(), blobData.data(), blobData.size());
		memcpy(bufferParse.data() + blobData.size(), buffer_.data(), transferred);
		buffer_.swap(bufferParse);
		transferred += blobData.size();
	}

	/// The incoming request.
	http::parser::request request;

	http::parser::result_type result;
	char* endData = 0x00;

	/// The parser for the incoming request.
	std::tie(result, endData) = http::parser::Parse
	(
		request,
		buffer_.data(),
		buffer_.data() + transferred,
		true
	);

	const auto countParser = endData - buffer_.data();

	///Посчитать разницу между тем было и что получили если нехватает то дополучить
	const auto len = request.GetHeaderValue("Content-Length");
	const size_t sizeBufuffer = atoi(len.c_str());

	if (result == http::parser::indeterminate){
		// Start an asynchronous operation to read a newline-delimited message.
		BlobData ddd(buffer_.data(), buffer_.data() + transferred);
		socket_.async_read_some(boost::asio::buffer(buffer_), boost::bind(&Connect::HandleRead, this, connect, _1, _2, ddd));
		return;
	}


	std::wstringstream streamData;
	if (result != http::parser::good)
	{
		std::wcout << L"Invalid parse http header." << std::endl;
		Stop();
		return;
	}

	if ((countParser + sizeBufuffer) > transferred)
	{
		ArrayBuffer bufferFinal;
		memset(bufferFinal.data(), 0x00, bufferFinal.size());
		size_t sizeRead = 0;
		do
		{
			ArrayBuffer bufferData;
			memset(bufferData.data(), 0x00, bufferData.size());
			const auto size = socket_.read_some(boost::asio::buffer(bufferData));
			memcpy(bufferFinal.data() + sizeRead, bufferData.data(), size);
			sizeRead += size;
		} while (sizeRead < sizeBufuffer);

		streamData = std::wstringstream(boost::locale::conv::utf_to_utf<wchar_t>(bufferFinal.data()));
	}
	else
	{
		streamData = std::wstringstream(boost::locale::conv::utf_to_utf<wchar_t>(buffer_.data() + countParser));
	}

	std::wcout << L"Result client: " << streamData.str().c_str() << std::endl;
	//Осуществляем обработку данных
	service_->OnRequestResult(streamData, connectInfo_);
	//принудительно рвем соединение
	Stop();
}

void Connect::StartWrite(ConnectPtr connect)
{
	if (stopped_){
		return;
	}

	// Start an asynchronous operation to send a heartbeat message.
	auto stream = inquiry_->PostToBuffers();
	boost::asio::async_write(socket_, boost::asio::buffer(stream.str(), stream.str().length()), boost::bind(&Connect::HandleWrite, this, connect, _1));
}

void Connect::HandleWrite(ConnectPtr connect, const boost::system::error_code& ec)
{
	if (stopped_){
		return;
	}

	if (!ec)
	{
		// Wait 10 seconds before sending the next heartbeat.
		heartbeat_timer_.expires_from_now(boost::posix_time::seconds(10));
		heartbeat_timer_.async_wait(boost::bind(&Connect::StartWrite, this, connect));
	}
	else
	{
		std::cout << "Error on heartbeat: " << ec.message() << "\n";
		Stop();
	}
}

void Connect::CheckDeadline( ConnectPtr connect )
{
	if (stopped_){
		return;
	}

	// Check whether the deadline has passed. We compare the deadline against
	// the current time since a new asynchronous operation may have moved the
	// deadline before this actor had a chance to run.
	if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
	{
		// The deadline has passed. The socket is closed so that any outstanding
		// asynchronous operations are cancelled.
		socket_.lowest_layer().close();

		// There is no longer an active deadline. The expiry is set to positive
		// infinity so that the actor takes no action until a new deadline is set.
		deadline_.expires_at(boost::posix_time::pos_infin);
	}

	// Put the actor back to sleep.
	deadline_.async_wait(boost::bind(&Connect::CheckDeadline, this, connect));
}

void Connect::Stop()
{
	stopped_ = true;
	socket_.lowest_layer().close();
	deadline_.cancel();
	heartbeat_timer_.cancel();
}

bool Connect::VerifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx)
{
	// In this example we will simply print the certificate's subject name.
	char subject_name[1024];
	X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
	X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 1024);
	std::cout << "Verifying " << subject_name << "\n";

	return true;
}

void Connect::SendData(const rpc::info::IConnectInfoPtr& connectInfo, const http::parser::ReplyPtr& inquiry)
{
	inquiry_ = inquiry;
	connectInfo_ = connectInfo;

	auto connect = shared_from_this();
	//формируем описание подключения
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), connectInfo->GetHost(), connectInfo->GetPort());
	StartConnect(connect, resolver_.resolve(query));

	// Start the deadline actor. You will note that we're not setting any
	// particular deadline here. Instead, the connect and input actors will
	// update the deadline prior to each asynchronous operation.
	deadline_.async_wait(boost::bind(&Connect::CheckDeadline, this, connect));
}

Connect::Connect(const rpc::ServicePtr& service, boost::asio::io_service& ioService, boost::asio::ssl::context& context) :
service_(service), socket_(ioService, context), deadline_(ioService), heartbeat_timer_(ioService), resolver_(ioService), stopped_(false)
{
	socket_.set_verify_mode(boost::asio::ssl::verify_peer);
	socket_.set_verify_callback( boost::bind(&Connect::VerifyCertificate, this, _1, _2) );
}

Connect::~Connect()
{

}
