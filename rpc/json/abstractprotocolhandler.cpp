#include <rpc/include/precompiled.h>

namespace rpc
{
//=====================================================================================================================
AbstractProtocol::AbstractProtocol( const rpc::ProcedureManager& procedureManager ) : procedureManager_( procedureManager )
{
}
//=====================================================================================================================
AbstractProtocol::~AbstractProtocol()
{
}
//=====================================================================================================================
bool AbstractProtocol::ParseResult(const std::wstringstream &request, const rpc::info::IConnectInfoPtr& connectInfo) const
{
	if (request.str().empty()){
		return false;
	}

	try
	{
		std::wstringstream data(request.str());
		boost::property_tree::wptree ptree;

		boost::property_tree::read_json(data, ptree);

		const rpc::Value req(ptree);

		//It could be a Batch Request
		if (req.isArray()){
			return ParseBatchResult(req, connectInfo);
		}

		//It could be a simple Request
		if (req.isObject()){
			return ParseSingleResult(req, connectInfo);
		}
	}
	catch (const std::exception&)
	{	
		assert(!"invalid parse json answer.");
	}

	return false;
}
//=====================================================================================================================
std::wstringstream AbstractProtocol::Request(const std::wstringstream &request ) const
{
	rpc::ValueList respList;

	try
	{
		if( request.str().empty() ){
			throw std::runtime_error( "Invalid input data: Empty string." );
		}
		
		std::wstringstream data( request.str() );
		boost::property_tree::wptree ptree;
    
		boost::property_tree::read_json( data, ptree );

		const rpc::Value req( ptree );
		respList = RequestJson( req );
	}
	catch( const std::exception& expeption )
	{
		const auto code = rpc::enums::ERROR_RPC_JSON_PARSE_ERROR;

		std::wstringstream message;
		message << rpc::GetErrorMessage( code ) << L" ( " << expeption.what() << L" ) ";

		respList.push_back( WrapError( rpc::Value(), code, message.str() ) );
	}
 
	if( respList.empty() ){
		return std::wstringstream();
	}

	bool bFirst = true; 
	std::wstringstream result;
	for( const auto& resp : respList )
	{
		if( !resp.isValid() ){
			continue;
		}

		result << ( bFirst ? L"[\n" : L",\n" ); 
		bFirst = false;

		std::wstringstream respString;
		boost::property_tree::write_json( respString, resp() );
		result << respString.str().c_str();
	}

	result << L"]";

	return result;
}
//=====================================================================================================================
rpc::Value AbstractProtocol::ProcessRequest(const rpc::Value &request) const
{
	const auto procedure = procedureManager_[ request[KEY_REQUEST_METHODNAME].asString() ];
	if (!procedure){
		return WrapError(request, rpc::enums::ERROR_METHOD_NOT_FOUND, L"Invalid init method");
	}
	
	if( procedure->GetType() == rpc::enums::ProcedureType::method )
    {
		const rpc::Value& result = (*procedure)(request[KEY_REQUEST_PARAMETERS], rpc::Value(), rpc::info::IConnectInfoPtr());
        return WrapResult(request, result );
    }
    else
    {
		(*procedure)(request[KEY_REQUEST_PARAMETERS], rpc::Value(), rpc::info::IConnectInfoPtr());
        return rpc::Value();
    }
}
//=====================================================================================================================
bool AbstractProtocol::ProcessRequestError(const std::wstring& type, const rpc::Value &error, const rpc::info::IConnectInfoPtr& connectInfo) const
{
	const auto procedure = procedureManager_[type];
	if (!procedure){
		return false;
	}
	if (procedure->GetType() != rpc::enums::ProcedureType::result){
		return false;
	}

	(*procedure)(rpc::Value(), error, connectInfo);
	return true;
}
//=====================================================================================================================
bool AbstractProtocol::ProcessRequestResult(const std::wstring& type, const rpc::Value &request, const rpc::info::IConnectInfoPtr& connectInfo) const
{
	const auto procedure = procedureManager_[type];
	if (!procedure){
		return false;
	}
	if (procedure->GetType() != rpc::enums::ProcedureType::result){
		return false;
	}
	
	(*procedure)(request, rpc::Value(), connectInfo);
	return true;
}
//=====================================================================================================================
rpc::enums::ErrorCode AbstractProtocol::ValidateRequest( const rpc::Value &request ) const
{
    if( !ValidateRequestFields(request) ){
        return rpc::enums::ERROR_RPC_INVALID_REQUEST;
    }
  
	//Поиск метода
	const auto procedure = procedureManager_[ request[KEY_REQUEST_METHODNAME].asString() ];
	if( !procedure ){
		return rpc::enums::ERROR_METHOD_NOT_FOUND;
	}
    
	//Проверка на тип сообщения
	if( GetRequestType(request) == rpc::enums::method && procedure->GetType() == rpc::enums::notification ){
		return rpc::enums::ERROR_SERVER_PROCEDURE_IS_NOTIFICATION;
	}
	
	//Проверка на тип сообщения
	if( GetRequestType(request) == rpc::enums::notification && procedure->GetType() == rpc::enums::method){
		return rpc::enums::ERROR_SERVER_PROCEDURE_IS_METHOD;
	}

	return rpc::enums::ERROR_OK;
}
//=====================================================================================================================
}
