/// @file 
/// @brief Файл содержит описание классов для хранения свойств ошибок используемые при генерации исключений
#pragma once

namespace rpc
{
namespace value
{
/// @brief Описание исключения при разборе сообщения от сервера
///
class Exception : public std::exception
{
public:
	/// @brief Конструктор
	///
	/// @param[in] code Код ошибки
	explicit Exception( const rpc::enums::ErrorCode code );
	/// @brief Конструктор
	///
	/// @param[in] code код ошибки
	/// @param[in] message описание ошибки
	Exception( const rpc::enums::ErrorCode code, const std::wstring& message );
	/// @brief Деструктор
	///
    ~Exception() throw();
public:
	/// @brief Возвращает код ошибки
	///
    rpc::enums::ErrorCode GetCode( void ) const throw();
	/// @brief Возвращает описанеие исключения
	///
	const std::wstring& GetMessage( void ) const throw(); 
private:
	/// Описание исключения
	std::wstring message_;
	/// Код ошибки
    rpc::enums::ErrorCode code_;
};
}
}
