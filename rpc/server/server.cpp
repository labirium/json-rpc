/// @file 
/// @brief Файл содержит реализацию создания сервера для обработки соединений по TCP протоколу
#include <rpc/include/precompiled.h>

namespace http 
{
namespace server 
{
//=====================================================================================================================
Server::Server( const rpc::ServicePtr& service, const std::string& address, const std::string& port )
  : ioService_(),
    signals_( ioService_ ),
    acceptor_( ioService_ ),
    service_( service ),
	connectionManager_()
{
	boost::asio::ip::tcp::resolver resolver( ioService_ );
	boost::asio::ip::tcp::resolver::query query(boost::asio::ip::tcp::v4(), address, port );
	
	const boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
	acceptor_.open( endpoint.protocol() );
	acceptor_.set_option( boost::asio::ip::tcp::acceptor::reuse_address( true ) );
	acceptor_.bind( endpoint );
	acceptor_.listen();
}
//=====================================================================================================================
Server::~Server()
{
}
//=====================================================================================================================
void Server::Run( void )
{
	//Подготавливаем сигналы на завершение приложения
	DoAWaitStop();
	//Подготавливаем подключение
   	DoAccept();
	//Запускаем
	thread_.reset( new std::thread(boost::bind(&boost::asio::io_service::run, &ioService_)));
}
//=====================================================================================================================
void Server::Stop(void)
{
	acceptor_.close();

	//останавливаем сервис
	ioService_.stop();

	//Закрываем открытые соединения
	connectionManager_.StopAll();
	//Останавливаем поток
	thread_ ? thread_->join() : (void)thread_;
	thread_.reset();
}
//=====================================================================================================================
void Server::DoAccept( void )
{
	// поток данных
	auto stream = std::make_shared<SslStream>(ioService_);

	acceptor_.async_accept( stream->GetSocket(), [this, stream]( const boost::system::error_code& errorCode )
	{
		//Проверяем что сервис работает
		if( !acceptor_.is_open() || !stream ){
			return;
		}

		// Проверяем код ошибки
		if( !errorCode )
		{
			const auto connect = std::make_shared<Connection>( service_, stream, connectionManager_ );
			connectionManager_.Start( connect );
		}
		else
		{
			stream->GetSocket().close();
		}

		//Готовим новое подключе
		DoAccept();
	});
}
//=====================================================================================================================
void Server::DoAWaitStop( void )
{
	// Register to handle the signals that indicate when the server should exit.
	// It is safe to register for the same signal multiple times in a program,
	// provided all registration for the specified signal is made through Asio.

	signals_.add( SIGTERM );
	signals_.add( SIGABRT );	

	signals_.async_wait( [this]( const boost::system::error_code&, const int signal )
	{
		(void)signal;
		// The server is stopped by cancelling all outstanding asynchronous
		// operations. Once all operations have finished the io_service::run()
		// call will exit.
		acceptor_.close();
		//останавливаем сервис
		ioService_.stop();
		//Закрываем открытые соединения
		connectionManager_.StopAll();
	});
}
//=====================================================================================================================
} 
} 
