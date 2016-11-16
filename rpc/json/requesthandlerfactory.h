/// @file 
/// @brief Файл содержит объявление функций для получения интерфейса для обработки команд rpc
#pragma once

namespace rpc 
{
namespace enums
{
/// Версия JSON протокола
typedef enum 
{
	/// JSON-RPC 1.0 
	RPC_SERVER_V1,
	/// JSON-RPC 2.0
	RPC_SERVER_V2, 
	/// JSON-RPC 1.2
	RPC_SERVER_V12
} ServerVersion;
}

namespace factory
{
/// @brief Получения интерфейса для обработки команд rpc с учетом версии протокола
///
/// @param[in] procedureManager Менеджер функций
/// @param[in] version Версия протокола 
/// @return Возвращает протокол для обработки команд rpc
IProtocolPtr CreateProtocol( const rpc::ProcedureManager& procedureManager, const rpc::enums::ServerVersion version );
/// @brief Получения интерфейса для обработки команд rpc с учетом версии
///
/// version Шаблон версия протокола 
/// @param[in] procedureManager Менеджер функций
/// @return Возвращает протокол для обработки команд rpc
template< rpc::enums::ServerVersion version >
IProtocolPtr CreateProtocol( const rpc::ProcedureManager& procedureManager );
}
} 
