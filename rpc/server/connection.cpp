#include <rpc/include/precompiled.h>
#include <rpc/include/filesystem.h>

namespace http 
{
namespace server 
{
//=====================================================================================================================
// TcpStream
//=====================================================================================================================
TcpStream::Stream& TcpStream::GetStream( void )
{
	return stream_;
}
//=====================================================================================================================
TcpStream::Socket& TcpStream::GetSocket( void )
{
	return stream_;
}
//=====================================================================================================================
TcpStream::TcpStream( boost::asio::io_service& ioService ) : 
	stream_( ioService )
{	
}
//=====================================================================================================================
TcpStream::~TcpStream()
{
}
//=====================================================================================================================
//Ssl Stream
//=====================================================================================================================
SslStream::Stream& SslStream::GetStream( void )
{
	return stream_;
}
//=====================================================================================================================
SslStream::Socket& SslStream::GetSocket( void )
{
	return stream_.lowest_layer();
}
//=====================================================================================================================
/// Класс описывает свойства подключения
class SslProfile
{
public:
	/// @brief crt файл
	///
	const std::wstring& GetCrtFile( void ) const;
	/// @brief key файл
	///
	const std::wstring& GetKeyFile(void) const;
	/// @brief pem файл
	///
	const std::wstring& GetPemFile(void) const;
public:
	/// @brief Конструктор
	///
	/// @param[in] crtFile crt файл
	/// @param[in] keyFile key файл
	/// @param[in] pemFile pem файл
	SslProfile( const std::wstring& crtFile, const std::wstring& keyFile, const std::wstring& pemFile );
	/// @brief Деструктор
	///
	~SslProfile();
private:
	/// crt файл
	const std::wstring crtFile_;
	/// key файл
	const std::wstring keyFile_;
	/// pem файл
	const std::wstring pemFile_;
};

const std::wstring& SslProfile::GetCrtFile( void ) const
{
	return crtFile_;
}

const std::wstring& SslProfile::GetKeyFile( void ) const
{
	return keyFile_;
}

const std::wstring& SslProfile::GetPemFile( void ) const
{
	return pemFile_;
}

SslProfile::SslProfile(const std::wstring& crtFile, const std::wstring& keyFile, const std::wstring& pemFile)
	: crtFile_(crtFile), keyFile_(keyFile), pemFile_(pemFile)
{
}

SslProfile::~SslProfile()
{
}

//=====================================================================================================================
boost::asio::ssl::context CreateContext( const boost::asio::ssl::context::method method )
{
	boost::asio::ssl::context context( method );

	context.set_options(
        boost::asio::ssl::context::default_workarounds
         | boost::asio::ssl::context::no_sslv2
		 | boost::asio::ssl::context::no_sslv3
		// | boost::asio::ssl::context::no_tlsv1
	    // | boost::asio::ssl::context::no_tlsv1_1
		// | boost::asio::ssl::context::no_tlsv1_2
        | boost::asio::ssl::context::single_dh_use
		| boost::asio::ssl::context::no_compression);

	//see http://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/reference/ssl__context.html

	const boost::filesystem::path& folerPath = rpc::filesystem::GetFolderWork();
	const boost::filesystem::path& server_crt = folerPath / L"server.crt";
	const boost::filesystem::path& server_key = folerPath / L"server.key";
	const boost::filesystem::path& dh1024_pem = folerPath / L"dh1024.pem";
	const boost::filesystem::path& client_crt = folerPath / L"client.crt";

	std::wcout << L"program data path" << folerPath.string().c_str() << std::endl;

	context.use_certificate_chain_file(server_crt.string());
	context.use_private_key_file(server_key.string(), boost::asio::ssl::context::pem);
	context.use_tmp_dh_file(dh1024_pem.string());

	context.set_verify_mode(boost::asio::ssl::context::verify_peer | boost::asio::ssl::context::verify_fail_if_no_peer_cert);
	//Load a certification authority file for performing verification
	context.load_verify_file(client_crt.string());

	return context;
}
//=====================================================================================================================
SslStream::SslStream( boost::asio::io_service& ioService, const boost::asio::ssl::context::method method )
	: stream_( ioService, CreateContext( method ) )
{
}
//=====================================================================================================================
SslStream::~SslStream()
{

}
//=====================================================================================================================
// Connection
//=====================================================================================================================
Connection::Connection( const rpc::ServicePtr& service, const IStreamPtr& stream,
    ConnectionManager& manager)
  : service_( service ),
	stream_( stream ),
    ConnectionManager_(manager)
{

}

void Connection::Start( void )
{
	//рукопожатие
	stream_->GetStream().async_handshake
	(
		boost::asio::ssl::stream_base::server,
		[this]( const boost::system::error_code& errorCode )
		{
			if (!errorCode)
			{
				do_read( stream_, service_ );
			}
		    else
			{
				std::cout << errorCode.message().c_str() << "  value:"<< errorCode.category().name()  << std::endl;

				do_write( stream_, http::parser::reply::stock_reply( http::parser::reply::unauthorized ) );
			}	
		}
	);
}

void Connection::Stop( void )
{
	if( stream_ ){
		stream_->GetSocket().close();
	}
}

void Connection::do_read( const IStreamPtr& stream, const rpc::ServicePtr& service  )
{
	assert( stream && "Invslid data." );

	memset(buffer_.data(), 0x00, buffer_.size());

	auto self(shared_from_this());
	
	stream->GetStream().async_read_some
	(		
		boost::asio::buffer(buffer_),
		[self, stream, service](const boost::system::error_code& errorCode, const std::size_t transferred)
		{
			ArrayBuffer& buffer = self->buffer_;

			if( !service )
			{
				self->do_write(stream, http::parser::reply::stock_reply(http::parser::reply::no_content));
				return;
		    }
			std::cout << buffer.data() << std::endl;
			//Проверка на наличие ошибки
			if( boost::asio::error::operation_aborted == errorCode ){
				return;
			}
					 
			if( errorCode )
			{
			    auto ff = errorCode.value();	
		     	auto msg = errorCode.message();
				self->ConnectionManager_.Stop(self);
				return;
			}
        
			/// The incoming request.
			http::parser::request request;

		    http::parser::result_type result;
		    char* endData = 0x00;

			 /// The parser for the incoming request.
          std::tie(result, endData) = http::parser::Parse
		  (
              request, 
			  buffer.data(),
			  buffer.data() + transferred
		  );

		  const auto countParser = endData - buffer.data();

		  ///Посчитать разницу между тем было и что получили если нехватает то дополучить
		  const auto len = request.GetHeaderValue( "Content-Length" );
		  const size_t sizeBufuffer = atoi( len.c_str() );

		  std::wstringstream streamData;
		  if (result == http::parser::good)
          {
			  if( ( countParser + sizeBufuffer ) > transferred )
			  {
				  ArrayBuffer bufferFinal;
				  memset(bufferFinal.data(), 0x00, bufferFinal.size());
				  size_t sizeRead = 0;
				  do
				  {
					  ArrayBuffer bufferData;
					  memset(bufferData.data(), 0x00, bufferData.size());
					  const auto size = stream->GetStream().read_some(boost::asio::buffer(bufferData));
					  memcpy(bufferFinal.data() + sizeRead, bufferData.data(), size);
					  sizeRead+=size;
				  }
				  while( sizeRead < sizeBufuffer );
		
		    	  streamData = std::wstringstream( boost::locale::conv::utf_to_utf<wchar_t>( bufferFinal.data() ) );
			  }
			  else
			  {
				 streamData = std::wstringstream( boost::locale::conv::utf_to_utf<wchar_t>( buffer.data() + countParser ) );
			  }
			  
			  std::wcout << streamData.str().c_str() << std::endl;
			  const auto reply = service->OnRequest( streamData );

			  std::wcout << reply->content.data() << std::endl;
			  self->do_write( stream, reply );
          }
          else if (result == http::parser::bad)
          {
			  const auto reply = http::parser::reply::stock_reply(http::parser::reply::bad_request);
			  self->do_write(stream, reply);
          }
          else
          {
			  self->do_read(stream, service);
          }
      });
}

void Connection::do_write( const IStreamPtr& stream, const http::parser::ReplyPtr& data )
{
	assert( stream && "Invslid data." );

    auto self(shared_from_this());
	auto buffer = data->ToBuffers();
	
	boost::asio::async_write
	(  
		stream->GetStream(), 
		boost::asio::buffer(buffer.str(), buffer.str().length()),
        [self, data, stream](boost::system::error_code ec, std::size_t size )
        {
			if( !ec )
			{ 
				// Закрытие соединения
				boost::system::error_code ignored_ec;
				stream->GetSocket().shutdown(boost::asio::basic_stream_socket<SslStream::Protocol>::shutdown_both, ignored_ec);
			}

			if ( ec != boost::asio::error::operation_aborted )
			{
				self->ConnectionManager_.Stop(self);
			}
      });
}

} 
} 
