#pragma once

namespace http
{
namespace server
{
	
class TcpStream : boost::noncopyable
{
public:
	typedef boost::asio::ip::tcp Protocol;
	typedef boost::asio::basic_stream_socket<Protocol> Stream;
	typedef boost::asio::basic_socket<Protocol, boost::asio::stream_socket_service<Protocol>> Socket;
public:
	explicit TcpStream( boost::asio::io_service& ioService );

	~TcpStream();
public:
	Stream& GetStream( void );
	
	Socket& GetSocket( void );
private:
	Stream stream_;
};

class SslStream : boost::noncopyable
{
public:
	typedef boost::asio::ip::tcp Protocol;
	typedef boost::asio::ssl::stream<boost::asio::basic_stream_socket<Protocol>> Stream;
	typedef boost::asio::basic_socket<Protocol, boost::asio::stream_socket_service<Protocol>> Socket;
public:
	SslStream(boost::asio::io_service& ioService, const boost::asio::ssl::context::method method = boost::asio::ssl::context::tlsv12_server);
	~SslStream();
public:
	Stream& GetStream( void );
	Socket& GetSocket( void );
private:
	Stream stream_;	
};

#define SIZE_BUFFER 8192
/// Описание временного буфера
typedef std::array<char, SIZE_BUFFER> ArrayBuffer;

class ConnectionManager;

/// Represents a single connection from a client.
class Connection : public std::enable_shared_from_this<Connection>
{
public:
	/// @brief Конструктор
	///
	/// @param[in] service Сервис
	/// @param[in] поток данных
	Connection( const rpc::ServicePtr& service, const IStreamPtr& stream, ConnectionManager& manager);
public:
	/// @brief Start the first asynchronous operation for the connection.
    ///
	void Start( void );
	/// @brief Stop all asynchronous operations associated with the connection.
	///
	void Stop( void );
private:
	/// @brief Perform an asynchronous read operation.
	///
	/// @param[in] stream поток данных
	/// @param[in] service сервис
	void do_read( const IStreamPtr& stream, const rpc::ServicePtr& service );
	/// @brief Perform an asynchronous write operation.
	///
	/// @param[in] data результат на отправку
	void do_write( const IStreamPtr& stream, const http::parser::ReplyPtr& data );
private:
	/// поток данных
	IStreamPtr stream_;
	/// сервис
	rpc::ServicePtr service_;
  	/// The manager for this connection.
	ConnectionManager& ConnectionManager_;
	/// Буфер для обработки данных
	ArrayBuffer buffer_;
};

} // namespace server
} // namespace http

