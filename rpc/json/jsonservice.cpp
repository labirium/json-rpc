#include <rpc/include/precompiled.h>

namespace rpc
{
//=====================================================================================================================
http::parser::ReplyPtr Service::Inquiry(const rpc::client::CommandList& commands) const
{
	assert(protocol_ && "Invalid init type protocol.");

	if (commands.empty()){
		return http::parser::ReplyPtr();
	}

	std::wstringstream response;
	response << L"[\n";

	bool bFirst = true;
	for (const auto cmd : commands)
	{
		response << (bFirst ? L"" : L",\n");
		response << protocol_->CreateRequest(cmd->GetFunctionName(), cmd->GetValue(), cmd->GetCallbackFunctionName()).str();
		bFirst = false;
	}

	response << L"]";
	return CreateResponse(response);
}
//=====================================================================================================================
http::parser::ReplyPtr Service::OnRequest(const std::wstringstream &request) const
{
	assert( protocol_ && "Invalid init type protocol.");

	const auto& response = protocol_->Request( request );
    return CreateResponse( response );
}
//=====================================================================================================================
void Service::OnRequestResult(const std::wstringstream &request, const rpc::info::IConnectInfoPtr& connectInfo) const
{
	assert(protocol_ && "Invalid init type protocol.");
	protocol_->ParseResult(request, connectInfo);
}
//=====================================================================================================================
http::parser::ReplyPtr Service::CreateResponse( const std::wstringstream & response ) const
{
	if( response.str().empty() ){
		//внутренняя ошибка сервера
		return http::parser::reply::stock_reply( http::parser::reply::internal_server_error );
	}

	//Формируем ответ
	auto result = std::make_shared<http::parser::reply>();
	result->status = http::parser::reply::ok;
	result->content = boost::locale::conv::utf_to_utf<char>( response.str() ).c_str(), response.str().length() * sizeof( std::wstringstream::char_type );
	result->headers.resize( 2 );
	result->headers[0].name = "Content-Length";
	result->headers[0].value = std::to_string( response.str().length() );
	result->headers[1].name = "Content-Type";
	result->headers[1].value = http::server::mime_types::ExtensionToType("html");

	return result;
}
//=====================================================================================================================
Service::Service( const rpc::ProcedureManager& procedureManager, const rpc::enums::ServerVersion type ) : protocol_( rpc::factory::CreateProtocol( procedureManager, type ) )
{
}
//=====================================================================================================================
Service::~Service()
{
}
//=====================================================================================================================
}
