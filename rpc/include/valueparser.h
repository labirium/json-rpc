/// @file 
/// @brief Файл описывает шаблон по разбору данных полученные по протоколу rpc
#pragma once

namespace rpc
{
namespace value
{
/// @brief Шаблон осуществляет разбор данных переменной полученые по протоколу rpc
///
/// Type тип данных
template<typename Type>
struct Parser
{
	/// @brief Преобразует данные из одного типа в заданный
	///
	/// @param[in] value данные полученные по rpc
	/// @return Возвращает преобразованные данные в заданный тип
	static Type Get( const rpc::Value& value );
};
//=====================================================================================================================
/// @brief Шаблон осущеставляет поиск и разбор данных полученые по протоколу rpc
///
/// Type тип данных
///
/// param[in] name имя переменной
/// param[in] paramets параметры в котором требуется осуществить поиск данных
/// @return Возвращает преобразованные данные в заданный тип
template<typename Type>
Type Parametr( const std::wstring& name, const rpc::Value& paramets ) 
{
	//Проверка входных данных
	if( !paramets.isValid() ){
		throw rpc::value::Exception( rpc::enums::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, name );
	}

	// Проверяем наличие переменной
	if( !paramets.isMember( name ) ){
		throw rpc::value::Exception( rpc::enums::ERROR_RPC_INVALID_PARAMS, name );
	}

	//Получаем переменную
	const auto enable = paramets[ name ];
	if( !enable.isValid() ){
		throw rpc::value::Exception( rpc::enums::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, name );
	}

	//Получаем данные
	return rpc::value::Parser<Type>::Get( enable );
}
//=====================================================================================================================
/// @brief Шаблон осущеставляет поиск и разбор данных полученые по протоколу rpc
///
/// Type тип данных
///
/// param[in] paramets параметры в котором требуется осуществить поиск данных
/// @return Возвращает преобразованные данные в заданный тип
template<typename Type>
Type ParametrResult(const rpc::Value& paramets)
{
	//Проверка входных данных
	if (!paramets.isValid() || paramets.isNull()){
		throw rpc::value::Exception(rpc::enums::ERROR_SERVER_PROCEDURE_SPECIFICATION_SYNTAX, KEY_RESPONSE_RESULT);
	}

	//Получаем данные
	return rpc::value::Parser<Type>::Get(paramets);
}
//=====================================================================================================================
/// @brief Шаблон описывает упаковку данных для возврата результата
///
/// Type тип данных
///
/// param[in] data данные наупаковку
template<typename Type >
rpc::Value Result( const Type& data );
//=====================================================================================================================
}
}
