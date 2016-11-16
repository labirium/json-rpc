#pragma once

namespace rpc {


class AbstractProtocol : public IProtocol
{
public:
    explicit AbstractProtocol( const rpc::ProcedureManager& procedureManager );
    virtual ~AbstractProtocol();

	std::wstringstream Request(const std::wstringstream &request ) const;
protected:
	/// @brief Осуществляет преобразования типа запроса к заданной функции
	///
	/// @param[in] request описание запроса
    rpc::Value ProcessRequest( const rpc::Value &request ) const;
	/// @brief Осуществляет преобразования типа запроса к заданной функции для результата операции
	///
	/// @param[in] type тип запроса
	/// @param[in] error описание ошибки
	/// @param[in] connectInfo описание подключения
	bool ProcessRequestResult(const std::wstring& type, const rpc::Value &error, const rpc::info::IConnectInfoPtr& connectInfo) const;
	/// @brief Осуществляет обработку ошибки при получении ответа на запрос
	///
	/// @param[in] type тип запроса
	/// @param[in] request описание запроса
	/// @param[in] connectInfo описание подключения
	bool ProcessRequestError(const std::wstring& type, const rpc::Value &request, const rpc::info::IConnectInfoPtr& connectInfo) const;

    rpc::enums::ErrorCode ValidateRequest(const rpc::Value &val) const;
	/// @brief Осуществляет разбор группы сообщений
	///
	/// @param[in] request сообщение на обработку
	/// @param[in] connectInfo описание подключения
	bool ParseResult(const std::wstringstream &request, const rpc::info::IConnectInfoPtr& connectInfo) const;
protected:
    virtual rpc::ValueList RequestJson( const rpc::Value& request ) const = 0;
    virtual bool ValidateRequestFields(const rpc::Value &val) const = 0;
    virtual rpc::Value WrapResult(const rpc::Value& request, const rpc::Value& result ) const = 0;
    virtual rpc::Value WrapError(const rpc::Value& request, rpc::enums::ErrorCode code, const std::wstring &message ) const = 0;
    virtual rpc::enums::ProcedureType GetRequestType(const rpc::Value& request) const = 0;
	/// @brief Осуществляет создание json запроса
	///
	/// @param[in] name имя функции
	/// @param[in] params список параметров
	/// @param[in] number уникальный номер сообщения
	/// @retrn Возвращает сформированный запрос на отправку 
	virtual std::wstringstream CreateRequest(const std::wstring& name, const rpc::Value& params, const std::wstring& number) const = 0;
	/// @brief Оссуществляет разбор сообщения
	///
	/// @param[in] request сообщение на обработку
	/// @param[in] connectInfo описание подключения
	virtual bool ParseSingleResult(const rpc::Value &request, const rpc::info::IConnectInfoPtr& connectInfo) const = 0;
	/// @brief Осуществляет разбор результата запроса
	///
	/// @param[in] req сообщение на обработку
	/// @param[in] connectInfo описание подключения
	virtual bool ParseBatchResult(const rpc::Value &req, const rpc::info::IConnectInfoPtr& connectInfo) const = 0;
private:
	/// Список процедур
	rpc::ProcedureManager procedureManager_;
};

} 

