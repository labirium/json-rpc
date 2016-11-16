/// @file 
/// @brief Файл содержит реализацию функций для работы с ошибками
#include <rpc/include/precompiled.h>

namespace
{
/// Список для хранения описания ошибки 
typedef std::map<rpc::enums::ErrorCode, std::wstring> ErrorList;

/// @brief Возвращает список зарегестрированных ошибок
///
ErrorList GetErrorCodeList( void )
{
	ErrorList errorList;
	errorList.insert( std::make_pair( rpc::enums::ERROR_RPC_INVALID_REQUEST, L"INVALID_JSON_REQUEST: The JSON sent is not a valid JSON-RPC Request object" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_METHOD_NOT_FOUND, L"METHOD_NOT_FOUND: The method being requested is not available on this Server" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_RPC_INVALID_PARAMS, L"INVALID_PARAMS: Invalid method parameters (invalid name and/or type) recognised" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_RPC_JSON_PARSE_ERROR,  L"JSON_PARSE_ERROR: The JSON-Object is not JSON-Valid" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_RPC_INTERNAL_ERROR, L"INTERNAL_ERROR: " ) );

    errorList.insert( std::make_pair( rpc::enums::ERROR_SERVER_PROCEDURE_IS_METHOD, L"PROCEDURE_IS_METHOD: The requested notification is declared as a method" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_SERVER_PROCEDURE_IS_NOTIFICATION, L"PROCEDURE_IS_NOTIFICATION: The requested method is declared as notification" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_SERVER_PROCEDURE_POINTER_IS_NULL, L"PROCEDURE_POINTER_IS_NULL: Server has no function Reference registered: " ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND, L"Configuration file was not found: " ) );

    errorList.insert( std::make_pair( rpc::enums::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, L"Procedure specification mallformed: " ) );

    errorList.insert( std::make_pair( rpc::enums::ERROR_CLIENT_INVALID_RESPONSE, L"The response is invalid" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_CLIENT_CONNECTOR, L"Client connector error" ) );
    errorList.insert( std::make_pair( rpc::enums::ERROR_SERVER_CONNECTOR, L"Server connector error" ) );

	return errorList;
}
}

namespace rpc
{   
//========================================================================================================================
std::wstring GetErrorMessage( const rpc::enums::ErrorCode errorCode )
{
	//Список ошибок
	static ErrorList errorList;

	//Иницилизация списка
	static std::once_flag onceFlag;
	std::call_once( onceFlag, []()
	{
		errorList = GetErrorCodeList();
	} );

	const auto iterator = errorList.find( errorCode );
	if( iterator == errorList.end() )
	{
		assert( !"Invalid input code error." );
		return std::wstring();
	}

	//Возвращаем описание ошибки
	return iterator->second;
}
//========================================================================================================================
}
