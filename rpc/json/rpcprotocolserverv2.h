#pragma once

#define KEY_REQUEST_VERSION     L"jsonrpc"
#define JSON_RPC_VERSION2        L"2.0"

namespace rpc
{
class RpcProtocolServerV2 : public AbstractProtocol
{
public:
	/// @brief Конструктор
	///
    explicit RpcProtocolServerV2( const rpc::ProcedureManager& procedureManager );
	/// @brief Деструктор
	///
	~RpcProtocolServerV2();
public:
    rpc::ValueList RequestJson( const rpc::Value& request ) const;
    bool ValidateRequestFields(const rpc::Value &val) const;
	/// @brief Осуществляет проверку формата для ответного сообщения
	///
	bool ValidateRequestFieldsResult(const rpc::Value &request) const;
    rpc::Value WrapResult(const rpc::Value& request, const rpc::Value& result) const;
    rpc::Value WrapError(const rpc::Value& request, rpc::enums::ErrorCode code, const std::wstring &message) const;
    rpc::enums::ProcedureType GetRequestType(const rpc::Value& request) const;
	/// @brief Осуществляет создание json запроса
	///
	/// @param[in] name имя функции
	/// @param[in] params список параметров
	/// @param[in] number уникальный номер сообщения
	/// @retrn Возвращает сформированный запрос на отправку 
	std::wstringstream CreateRequest(const std::wstring& name, const rpc::Value& params, const std::wstring& number) const;
private:
	/// @brief Осуществляет разбор результата запроса
	///
	/// @param[in] request сообщение на обработку
	/// @param[in] connectInfo описание подключения
	bool ParseSingleResult(const rpc::Value &request, const rpc::info::IConnectInfoPtr& connectInfo) const;
	/// @brief Осуществляет разбор результата запроса
	///
	/// @param[in] req сообщение на обработку
	/// @param[in] connectInfo описание подключения
	bool ParseBatchResult(const rpc::Value &req, const rpc::info::IConnectInfoPtr& connectInfo) const;

    rpc::Value SingleRequest(const rpc::Value& request) const;
    rpc::ValueList BatchRequest(const rpc::Value& requests) const;
};

} 

