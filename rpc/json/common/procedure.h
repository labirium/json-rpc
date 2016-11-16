#pragma once

namespace rpc
{
/// @brief Класс описывает свойства процедуры
///
class Procedure
{	
public:
	/// @brief Конструктор
	///
	/// @param[in] name имя функции
	/// @param[in] callback ссылка на функцию
	Procedure( const std::wstring& name, const rpc::function::ICallbackPtr& callback );
	/// @brief Деструктор
	///
	~Procedure();
public:
	/// @brief Возвращает имя функции
	///
	const std::wstring& GetName( void ) const;
	/// @brief Возвращает тип роцедуры
	///
	rpc::enums::ProcedureType GetType( void ) const;
public:
	/// @brief Осуществляет выполнение зарегистрированной функции
	///
	rpc::Value operator()(const rpc::Value& input, const rpc::Value& error, const rpc::info::IConnectInfoPtr& connectInfo);
private:
	/// сылка на функцию
	rpc::function::ICallbackPtr callback_;
	/// имя функции
	std::wstring name_;
};
}
