/// @file 
/// @brief Файл содержит описание создание сервера для обработки соединений по TCP протоколу
#pragma once

namespace http 
{
namespace server 
{

class Server;
typedef std::shared_ptr<Server> ServerPtr;

/// @brief Класс осуществляет организацию сервера для подключения по TCP протоколу
///
class Server
{
public:
	/// @brief Конструктор
	///
	/// @param[in] service Описание сервера
	/// @param[in] address IP адрес сервера
	/// @param[in] port порт сервера
	Server( const rpc::ServicePtr& service, const std::string& address, const std::string& port );
	/// @brief Деструктор
	///
	~Server();
public:
	/// @brief Запуск сервера
	///
	void Run( void );
	/// @brief Остановка сервиса
	///
	void Stop(void);
private:
	/// @brief Регистрация асинхронного подключения
	///
	void DoAccept( void );
	/// @brief Регистрация на останоку сервера
	///
	void DoAWaitStop( void );
private:
	/// сервис для выполнения асинхронных операций
	boost::asio::io_service ioService_;
	/// спользуется для регистрации для завершения процесса уведомления
	boost::asio::signal_set signals_;
	/// используется для прослушивания входящих соединений.
	boost::asio::ip::tcp::acceptor acceptor_;
private:
	/// поток
	std::unique_ptr<std::thread> thread_;
	/// Описание сервиса
	rpc::ServicePtr service_;
	/// менеджер активных соединений
	ConnectionManager connectionManager_; 
};

} // namespace server
} // namespace http
