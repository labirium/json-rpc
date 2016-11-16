#include <rpc/include/precompiled.h>

namespace rpc
{
//=====================================================================================================================
RpcProtocolServerV2::RpcProtocolServerV2( const rpc::ProcedureManager& procedureManager ) : AbstractProtocol( procedureManager )
{
}
//=====================================================================================================================
RpcProtocolServerV2::~RpcProtocolServerV2()
{
}
//=====================================================================================================================
rpc::ValueList RpcProtocolServerV2::RequestJson( const rpc::Value &req ) const
{
    //It could be a Batch Request
    if (req.isArray()){
        return BatchRequest( req );
    }
	
	//It could be a simple Request
    rpc::ValueList result;
	if (req.isObject())
	{	
		result.push_back( SingleRequest( req ) );
		return result;
    }
    
	const auto code = rpc::enums::ERROR_RPC_INVALID_REQUEST;
	result.push_back( WrapError( rpc::Value(), code, rpc::GetErrorMessage( code ) ) );
	return result;
}
//=====================================================================================================================
rpc::Value RpcProtocolServerV2::SingleRequest( const rpc::Value &req ) const
{
	const auto error = ValidateRequest( req );
	if (error != 0){
		return WrapError(req, error, rpc::GetErrorMessage( error ) );
	}
    
	try
    {
        return ProcessRequest( req );
    }
	catch( const rpc::value::Exception& exception )
    {
        return WrapError(req, exception.GetCode(), exception.GetMessage() );
    }
}
//=====================================================================================================================
rpc::ValueList RpcProtocolServerV2::BatchRequest( const rpc::Value &req ) const
{
	rpc::ValueList valueList;
    if( req.isEmpty() )
	{
		const auto& result = WrapError( rpc::Value(),  rpc::enums::ERROR_RPC_INVALID_REQUEST, rpc::GetErrorMessage( rpc::enums::ERROR_RPC_INVALID_REQUEST) );
		valueList.push_back( result );
        return valueList;
	}

	for( const auto& data : req() )
    {
		const rpc::Value& result = SingleRequest( rpc::Value( data.second ) );
		if( result.isValid() ){
            valueList.push_back( result );
		}
    }

	return valueList;
}
//=====================================================================================================================
bool RpcProtocolServerV2::ValidateRequestFields( const rpc::Value &request ) const
{
    if (!request.isObject()){
        return false;
	}
    
	if (!(request.isMember(KEY_REQUEST_METHODNAME) && request[KEY_REQUEST_METHODNAME].isString())){
        return false;
	}

    if (!(request.isMember(KEY_REQUEST_VERSION) && request[KEY_REQUEST_VERSION].isString() && request[KEY_REQUEST_VERSION].asString() == JSON_RPC_VERSION2)){
        return false;
	}

    if (request.isMember(KEY_REQUEST_ID) && !(request[KEY_REQUEST_ID].isInt() || request[KEY_REQUEST_ID].isString() || request[KEY_REQUEST_ID].isNull())){
        return false;
	}

    if (request.isMember(KEY_REQUEST_PARAMETERS) && !(request[KEY_REQUEST_PARAMETERS].isObject() || request[KEY_REQUEST_PARAMETERS].isArray() || request[KEY_REQUEST_ID].isNull())){
        return false;
	}
  
	return true;
}
//=====================================================================================================================
bool RpcProtocolServerV2::ValidateRequestFieldsResult(const rpc::Value &request) const
{
	if (!request.isObject()){
		return false;
	}

	if (!(request.isMember(KEY_REQUEST_VERSION) && request[KEY_REQUEST_VERSION].isString() && request[KEY_REQUEST_VERSION].asString() == JSON_RPC_VERSION2)){
		return false;
	}

	if (request.isMember(KEY_REQUEST_ID) && !(request[KEY_REQUEST_ID].isInt() || request[KEY_REQUEST_ID].isString() || request[KEY_REQUEST_ID].isNull())){
		return false;
	}

	if (!request.isMember(KEY_RESPONSE_RESULT) && !request.isMember(KEY_RESPONSE_ERROR) ){
		return false;
	}

	return true;
}
//=====================================================================================================================
rpc::Value RpcProtocolServerV2::WrapResult(const rpc::Value &request, const rpc::Value& result ) const
{
	boost::property_tree::wptree data;
	data.put( KEY_REQUEST_VERSION, JSON_RPC_VERSION2 );
	
	if( result.isValid() ){
		data.push_back( std::make_pair( KEY_RESPONSE_RESULT, result[KEY_RESPONSE_RESULT]() ) );
	}

	data.put( KEY_REQUEST_ID, request[KEY_REQUEST_ID].asString() );
	return rpc::Value( data );
}
//=====================================================================================================================
bool RpcProtocolServerV2::ParseBatchResult(const rpc::Value &req, const rpc::info::IConnectInfoPtr& connectInfo) const
{
	if (req.isEmpty()){
		return false;
	}

	for (const auto& data : req())
	{
		ParseSingleResult(rpc::Value(data.second), connectInfo);
	}

	return true;
}
//=====================================================================================================================
bool RpcProtocolServerV2::ParseSingleResult(const rpc::Value &request, const rpc::info::IConnectInfoPtr& connectInfo) const
{
	if (!ValidateRequestFieldsResult(request)){
		return false;
	}
	
	try
	{
		if (request.isMember(KEY_RESPONSE_RESULT)){
			return ProcessRequestResult(request[KEY_REQUEST_ID].asString(), request[KEY_RESPONSE_RESULT], connectInfo);
		}

		if (request.isMember(KEY_RESPONSE_ERROR)){
			return ProcessRequestError(request[KEY_REQUEST_ID].asString(), request[KEY_RESPONSE_ERROR], connectInfo);
		}

	}
	catch (const std::exception&)
	{
		assert(!"Invalid process request result.");
	}

	return false;
}
//=====================================================================================================================
rpc::Value RpcProtocolServerV2::WrapError( const rpc::Value &request, rpc::enums::ErrorCode code, const std::wstring &message ) const
{
	boost::property_tree::wptree error;
	error.put( L"code", code );
	error.put( L"message", message );

	boost::property_tree::wptree data;
	data.put( KEY_REQUEST_VERSION, JSON_RPC_VERSION2 );
	data.push_back( std::make_pair( L"error", error ) );
	data.put( KEY_REQUEST_ID, ( request.isValid() && request.isMember( KEY_REQUEST_ID ) && request[KEY_REQUEST_ID].isString() ) ? 
		request[KEY_REQUEST_ID].asString() : L"null" );

	return rpc::Value( data );
}
//=====================================================================================================================
std::wstringstream RpcProtocolServerV2::CreateRequest(const std::wstring& name, const rpc::Value& params, const std::wstring& number) const
{
	boost::property_tree::wptree data;
	data.put(KEY_REQUEST_VERSION, JSON_RPC_VERSION2);
	data.put(KEY_REQUEST_METHODNAME, name);

	if (params.isEmpty())
	{
		boost::property_tree::wptree paramNull;
		paramNull.put(L"", L"null");
		data.push_back(std::make_pair(KEY_REQUEST_PARAMETERS, paramNull));
	}
	else
	{
		data.push_back(std::make_pair(KEY_REQUEST_PARAMETERS, params()));
	}
	
	data.put(KEY_REQUEST_ID, number);

	std::wstringstream result;
	boost::property_tree::write_json(result, data);

	return result;
}
//=====================================================================================================================
rpc::enums::ProcedureType RpcProtocolServerV2::GetRequestType( const rpc::Value &request ) const
{
	if (request.isMember(KEY_RESPONSE_RESULT) || request.isMember(KEY_RESPONSE_ERROR)){
		return rpc::enums::result;
	}

    if (request.isMember(KEY_REQUEST_ID)){
		return rpc::enums::method;
	}

    return rpc::enums::notification;
}
//=====================================================================================================================
}
