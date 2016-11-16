/// @file 
/// @brief Файл содержит интерфейс протокола
#pragma once

namespace rpc
{
/// @brief Описание интерфейса для обработки запросов
class IProtocol
{
public:
	/// @brief Конструктор
	///
	IProtocol( void );
	/// @brief Декструктор
	///
    virtual ~IProtocol();
	/// @brief Осуществляет выполнения запроса
	///
	/// @param[in] request Строковый поток содержащий запрос
	/// @return Возвращает ответ на запрос
	virtual std::wstringstream Request( const std::wstringstream &request ) const = 0;
	/// @brief Осуществляет создание json запроса
	///
	/// @param[in] name имя функции
	/// @param[in] params список параметров
	/// @param[in] number уникальный номер сообщения
	/// @retrn Возвращает сформированный запрос на отправку 
	virtual std::wstringstream CreateRequest(const std::wstring& name, const rpc::Value& params, const std::wstring& number) const = 0;
	/// @brief Осуществляет разбор группы сообщений
	///
	/// @param[in] request запрос
	/// @param[in] connectInfo описание подключения
	virtual bool ParseResult(const std::wstringstream &request, const rpc::info::IConnectInfoPtr& connectInfo) const = 0;
};
}

