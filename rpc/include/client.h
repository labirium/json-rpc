#pragma once
#include <boost/asio/ssl.hpp>

namespace rpc
{ 
namespace client
{
#define SIZE_BUFFER 8192
/// Описание временного буфера
typedef std::array<char, SIZE_BUFFER> ArrayBuffer;
typedef  const std::vector<char> BlobData;

class Connect;
typedef std::shared_ptr<Connect> ConnectPtr;

typedef std::function<void(const std::wstringstream&)> CallbackAnswer;

/// @brief Класс осуществляет обработку подклбчения
///
class Connect : public std::enable_shared_from_this<Connect>
{
public:
	/// @brief Конструктор
	///
	/// @param[in] service описание обработчика комманд
	/// @param[in, out] ioService сервис для выполнения асинхронных операций
	/// @param[in, out] context описание свойств подключения
	Connect(const rpc::ServicePtr& service, boost::asio::io_service& ioService, boost::asio::ssl::context& context);
	/// @brief Деструктор
	///
	~Connect();
public:
	/// @brief Осуществляет отправку произольных данных
	///
	/// @param[in] connectInfo описание подключения
	/// @param[in] inquiry данные на отправку
	void SendData(const rpc::info::IConnectInfoPtr& connectInfo, const http::parser::ReplyPtr& inquiry);
private:
	/// @brief Обработчик для завершения подключения по истечению времени
	///
	void CheckDeadline(ConnectPtr connect);
	/// @brief Постановка задачи на отправку данных
	///
	void StartWrite(ConnectPtr connect);
	/// @brief Обработка отправки данных
	///
	void HandleWrite(ConnectPtr connect, const boost::system::error_code& ec);
	/// @brief Постановка задачи на чтение
	///
	void StartRead(ConnectPtr connect);
	/// @brief Обработка получения данных
	///
	void HandleRead(ConnectPtr connect, const boost::system::error_code& errorCode, std::size_t transferred, const BlobData& blobData);
	/// @brief Постановка задачи на подключение
	///
	void StartConnect(ConnectPtr connect, const boost::asio::ip::tcp::resolver::iterator& endpoint_iter);
	/// @brief Обработка подключения
	///
	void HandleConnect(ConnectPtr connect, const boost::system::error_code& ec, boost::asio::ip::tcp::resolver::iterator endpoint_iter);
	/// @brief Обработка рукопожатия
	///
	void HandleHandshake(ConnectPtr connect, const boost::system::error_code& error);
	/// @brief Осуществляет проверку сертификата при подключении по tls/ssl
	///
	bool VerifyCertificate(bool preverified, boost::asio::ssl::verify_context& ctx);
	/// @brief Остановка процесса
	///
	void Stop();
private:
	/// осуществляет обработку комманд
	rpc::ServicePtr service_;
	/// описание подключения
	rpc::info::IConnectInfoPtr connectInfo_;
private:
	/// данные на отправку
	http::parser::ReplyPtr inquiry_;
	/// Буфер для обработки данных
	ArrayBuffer buffer_;
private:
	/// обработчик заверщения подключения по таймеру
	boost::asio::deadline_timer deadline_;
	/// обработчик последующего подключения
	boost::asio::deadline_timer heartbeat_timer_;
	/// используется для резолвинга хоста
	boost::asio::ip::tcp::resolver resolver_;
	/// описание подключения
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
	/// признак завершения подключения
	bool stopped_;
};
}
}
