#pragma once


namespace rpc
{
namespace info
{
/// @brief Описание подключения
class IConnectInfo
{
public:
	/// @brief Конструктор
	///
	/// @param[in] host имя хоста
	/// @param[in] port порт подключения
	IConnectInfo(const std::string& host, const std::string& port);
	/// @brief Декструктор
	///
	~IConnectInfo();
public:
	/// @brief Возвращает имя хоста
	///
	const std::string& GetHost(void) const;
	/// @brief Возвращает порт подключения
	///
	const std::string& GetPort(void) const;
private:
	/// имя хоста
	std::string host_;
	/// порт подключения
	std::string port_;
};

/// @brief Свойства запроса
class IMessageInfo
{
public:
	/// @brief Конструктор
	///
	/// @param[in] connectInfo описание подключения
	/// @param[in] id идентификатор сообщения 
	IMessageInfo(const IConnectInfoPtr& connectInfo, const std::wstring& id);
	/// @brief Деструктор
	///
	~IMessageInfo();
public:
	/// @brief Вовзвращает описание подключения
	///
	const IConnectInfoPtr& GetIConnectInfo(void) const;
	/// @brief Возвращает идентификатор соощения 
	///
	const std::wstring& GetId( void ) const;
private:
	/// описание подключения
	IConnectInfoPtr connectInfo_;
	/// идентификатор сообщения 
	std::wstring id_;
};

}
}