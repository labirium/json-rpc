
#include <rpc/include/precompiled.h>
#include <rpc/include/client.h>
#include <assert.h>
#include <rpc/include/filesystem.h>

using namespace rpc;
using namespace rpc::client;

namespace
{

}

bool ClientManager::SendData(const std::string& address, const std::string& port, const CommandList& command )
{
	auto inquiry = service_->Inquiry(command);
	if (!inquiry){
		return false;
	}
	
	//Отправляем запрос
	auto connect = std::make_shared<ConnectPtr::element_type>(service_, ioService_, contex_);
	connect->SendData(std::make_shared<rpc::info::IConnectInfoPtr::element_type>(address, port), inquiry);

	return true;
}

void ClientManager::Run()
{	
	if (ioService_.stopped()){
		return;
	}

	//Запускаем
	thread_.reset(new std::thread(boost::bind(&boost::asio::io_service::run, &ioService_)));
}

void ClientManager::Stop()
{
	//останавливаем сервис
	ioService_.stop();
	//Останавливаем поток
	thread_ ? thread_->join() : (void)thread_;
	thread_.reset();
}

ClientManager::ClientManager( const rpc::ServicePtr& service ) 
	: ioService_(), contex_(boost::asio::ssl::context::tlsv12_client), service_(service), work_(ioService_)
{
	contex_.set_verify_mode(boost::asio::ssl::context::verify_peer | boost::asio::ssl::context::verify_fail_if_no_peer_cert);
	
	const boost::filesystem::path& folerPath = rpc::filesystem::GetFolderClient();
	const boost::filesystem::path& client_key = folerPath / L"client.key";
	const boost::filesystem::path& client_crt = folerPath / L"client.crt";

	contex_.use_private_key_file(client_key.string(), boost::asio::ssl::context::pem);
	contex_.use_certificate_file(client_crt.string(), boost::asio::ssl::context::pem);
}

ClientManager::~ClientManager()
{

}
