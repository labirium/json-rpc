/// @file 
/// @brief Файл содержит функции для работы с ошибками
#pragma once

namespace rpc
{   
namespace enums
{
/// Коды ошибок
typedef enum ErrorCode : int
{
	ERROR_OK = 0,
	//Official JSON-RPC 2.0 Errors
	
	/// Не разобрал JSON формат
	ERROR_RPC_JSON_PARSE_ERROR =  -32700,
	/// Ненайден метод
	ERROR_METHOD_NOT_FOUND =  -32601,
	ERROR_RPC_INVALID_REQUEST =   -32600,
	ERROR_RPC_INVALID_PARAMS =    -32602,
	ERROR_RPC_INTERNAL_ERROR =    -32603,
	//Server Library Errors
	ERROR_SERVER_PROCEDURE_IS_METHOD =                  -32604,
	ERROR_SERVER_PROCEDURE_IS_NOTIFICATION =            -32605,
	ERROR_SERVER_PROCEDURE_POINTER_IS_NULL =            -32606,
	ERROR_SERVER_PROCEDURE_SPECIFICATION_NOT_FOUND =    -32000,
	ERROR_SERVER_CONNECTOR =                            -32002,
	ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX =       -32007,
	// Client Library Errors
	ERROR_CLIENT_CONNECTOR =			-32003,
	ERROR_CLIENT_INVALID_RESPONSE =     -32001	
};
}

/// @brief Возваращет описание ошибки по коду
///
/// @param[in] errorCode код ошибки
/// @return Возвращает описание ошибки
std::wstring GetErrorMessage( const rpc::enums::ErrorCode errorCode );
} 
