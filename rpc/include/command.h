#pragma once

#include <rpc/json/value.h>

namespace rpc
{
namespace client
{

/// @brief Описание комманды
///
class Command : boost::noncopyable
{
public:
	/// @brief Конструктор
	///
	/// @param[in] function имя функции
	/// @param[in] value произвольные данные
	/// @param[in] callbackFunction имя функции которая будет обрабатывать ответ на запрос
	Command(const std::wstring& functionName, const rpc::Value& value, const std::wstring& callbackFunctionName);
	/// @brief Деструктор
	///
	~Command();
public:
	/// Возвращает имя функции
	///
	const std::wstring& GetFunctionName(void) const;
	/// Возвращает имя функции которая будут обрабаытывать ответ на запрос
	///
	const std::wstring& GetCallbackFunctionName(void) const;
	/// Возвращает произвольные данные
	///
	const rpc::Value& GetValue(void) const;
private:
	/// имя функции
	std::wstring functionName_;
	/// имя функции которая будет обрабаытвать ответ на запрос
	std::wstring callbackFunctionName_;
	/// произвольные данные
	rpc::Value value_;
};
}
}
