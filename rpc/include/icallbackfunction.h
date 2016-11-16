/// @file 
/// @brief Описание интерфеса обработки калбечной функции
#pragma once

namespace rpc
{
namespace function
{
//=====================================================================================================================
/// @brief Интерфейс описывающию обработку калбека
///
class ICallback
{
public:
	/// Тип переменной
	typedef std::wstring ValueName;
protected:
	/// @brief Конструктор
	///
	ICallback( void );
	/// @brief Деструктор
	///
	virtual ~ICallback();
public:
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @param[in] error Описание ошибки
	/// @param[in] connectInfo cвойства запроса
	/// @return Рузултат обработки функции
	virtual rpc::Value operator()(const rpc::Value& value, const rpc::Value& error, const rpc::info::IMessageInfoPtr& messaeInfo) = 0;
	/// @brief Возвращает тип реализации
	///
	virtual rpc::enums::ProcedureType Type( void ) const = 0;
};
//=====================================================================================================================
/// @brief Интерфейс описывающию обработку калбечной функции
///
class ICallbackFunction : public ICallback
{
protected:
	/// @brief Конструктор
	///
	ICallbackFunction( void );
	/// @brief Деструктор
	///
	virtual ~ICallbackFunction();
public:
	virtual rpc::enums::ProcedureType Type( void ) const;
};
//=====================================================================================================================
}
}
