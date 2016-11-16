/// @file 
/// @brief
#pragma once

#define KEY_REQUEST_METHODNAME  L"method"
#define KEY_REQUEST_ID          L"id"
#define KEY_REQUEST_PARAMETERS  L"params"
#define KEY_RESPONSE_ERROR      L"error"
#define KEY_RESPONSE_RESULT     L"result"
#define KEY_RESPONSE_CODE       L"code"
#define KEY_RESPONSE_MESSAGE    L"message"

namespace rpc
{
namespace enums
{
/// Описание типа процедуры
typedef enum ProcedureType
{
	/// тип метод
    method, 
	/// нотификация
	notification,
	/// обработка результата
	result
};
}
}
