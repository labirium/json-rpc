#pragma once
#include <rpc/include/command.h>

namespace rpc
{
/// @brief Осуществляет формирования запросов/ответов в формате RPC
///
class Service 
{
public:
	/// @brief Конструктор
	///
	/// @param[in] procedureManager манажер комманд
	/// @param[in] type тип протокола
    explicit Service( const rpc::ProcedureManager& procedureManager, const rpc::enums::ServerVersion type = rpc::enums::RPC_SERVER_V2 );
	/// @brief Деструктор
	///
    ~Service();
public:
	/// @brief Осуществляет обработку запроса
	///
	/// @param[in] request запрос
	/// @return Возвращает ответ на запрос
	http::parser::ReplyPtr OnRequest(const std::wstringstream& request) const;
	/// @brief Формирует запрос на отправку
	///
	/// @param[in] commands Список комманд
	/// @retrn Возвращает сформированный запрос на отправку 
	http::parser::ReplyPtr Inquiry(const rpc::client::CommandList& commands) const;
	/// @brief Осуществляет обработку результата ответа на запрос
	///
	/// @param[in] request запрос
	/// @param[in] connectInfo описание подключения
	void OnRequestResult(const std::wstringstream &request, const rpc::info::IConnectInfoPtr& connectInfo) const;
private:
	/// @brief Формирует http запрос
	///
	/// @param[in] request тело сообщения
	http::parser::ReplyPtr CreateResponse( const std::wstringstream& request ) const;
private:
	/// Описание протокола
    IProtocolPtr protocol_;
};

}

