/// @file 
/// @brief Файл содержит описание менеджера зарегестрированных функций

#pragma once

namespace rpc
{

/// @brief Менеджер хранит список зарегистрированных функций
class ProcedureManager
{
public:
	/// @brief Конструктор
	///
	ProcedureManager( void );
	/// @brief Деструктор
	///
	~ProcedureManager();
public:
	/// @brief Осуществляет добавление процедуры
	///
	/// @param[in] procedure Описание процедуры
	void Add( const rpc::Procedure& procedure );
	/// @brief Возвращаетфункцию по ее имени
	///
	/// @param[in] name Имя функции
	/// @return Описание функции
	rpc::ProcedurePtr operator[]( const std::wstring& name ) const;
	
private:
	/// Список зарегестрированных процедур
	rpc::ProcedureList procedureList_;
};
}
