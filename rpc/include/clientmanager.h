#pragma once
#include <boost/asio/ssl.hpp>

namespace rpc
{
namespace client
{
/// @brief Класс осуществляет управление над клиенскими подключениями
///
class ClientManager : boost::noncopyable
{
public:
	/// @brief Конструктор
	///
	/// @param[in] service описание обработчика комманд
	explicit ClientManager(const rpc::ServicePtr& service);
	/// @brief Деструктор
	///
	~ClientManager();
public:
	/// @brief Осуществляет отправку произвольных данных в формате RPC
	///
	/// @param[in] address Адрес подключения
	/// @param[in] port Порт подключения
	/// @param[in] command список комманд на отправку
	bool SendData(const std::string& address, const std::string& port, const CommandList& command);
	/// @brief Запуск сервиса
	///
	void Run();
	/// Остановка сервиса
	///
	void Stop();
private:
	/// Описание обработчика комманд
	rpc::ServicePtr service_;
	/// Контекст для подклбчения по ssl
	boost::asio::ssl::context contex_;
	/// сервис для выполнения асинхронных операций
	boost::asio::io_service ioService_;
	/// основной цикл для обработки данных
	std::unique_ptr<std::thread> thread_;
	/// осуществляет постановку задач
	boost::asio::io_service::work work_;
};

}
}
