/// @file 
/// @brief Файл описывает шаблоны для вызова зарегестрированной функции с N переменными
#pragma once

namespace rpc
{
namespace function
{
//=====================================================================================================================
/// @brief Шаблон осщуествляет вызов зарегестрированной функции с одной переменной
///
/// 
template<typename Type1>
class CallbackResult : public ICallbackFunction
{
	/// Описание функция для вызова
	typedef std::function< void(const Type1&, const rpc::info::IMessageInfoPtr& messageInfo) > Function;
	/// Обработка события ошибки на обработку запроса
	typedef std::function< void(const int code, const std::wstring& message, const rpc::info::IMessageInfoPtr& messageInfo) > FunktionError;
public:
	/// @brief Конструктор
	///
	/// @param[in] function функия на возврат результата обработки
	/// @param[in] error функия на вызов обработки ошибки
	CallbackResult(const Function& function, const FunktionError& error) : function_(function), error_(error)
	{
	}
	/// @brief Деструктор
	///
	~CallbackResult()
	{
	}
public:
	/// @brief Осуществляет возврат тип операции 
	rpc::enums::ProcedureType Type(void) const
	{
		return rpc::enums::result;
	}
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @param[in] error Описание ошибки
	/// @param[in] connectInfo описание подключения
	/// @return Рузултат обработки функции
	rpc::Value operator()(const rpc::Value& value, const rpc::Value& error, const rpc::info::IMessageInfoPtr& messageInfo)
	{
		//Обработка сообщения
		if (value.isValid() && !value.isNull())
		{
			const auto parametr1 = rpc::value::ParametrResult<Type1>(value);
			function_(parametr1, messageInfo);
			return value;
		}

		//Обработка ошибки
		if (error.isValid() && !error.isNull())
		{
			const int code = error.isMember(KEY_RESPONSE_CODE) ? error[KEY_RESPONSE_CODE].asInt() : -1;
			const std::wstring message = error.isMember(KEY_RESPONSE_MESSAGE) ? error[KEY_RESPONSE_MESSAGE].asString() : std::wstring();
			error_(code, message, messageInfo);
			return error;
		}
	
		return rpc::Value();
	}
private:
	///< Функция для вызова
	Function function_;
	///< Функция для вызова
	FunktionError error_;
};
//=====================================================================================================================
/// @brief Шаблон осщуествляет вызов зарегестрированной функции без переменных
///
/// 
template<typename ResultType>
class Callback0 : public ICallbackFunction
{
	/// Описание функция для вызова
	typedef std::function< ResultType ( void ) > Function;
public:
	/// @brief Конструктор
	///
	/// @param[in] function функия на вызов
	explicit Callback0( const Function& function ) 
		: function_( function )
	{
	}
	/// @brief Деструктор
	///
	~Callback0()
	{
	}
public:
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @return Рузултат обработки функции
	rpc::Value operator()(const rpc::Value& value, const rpc::Value&, const rpc::info::IMessageInfoPtr&)
	{
		return rpc::value::Result<ResultType>( function_() );
	}
private:
	///< Функция для вызова
	Function function_;	
};
//=====================================================================================================================
/// @brief Шаблон осщуествляет вызов зарегестрированной функции с одной переменной
///
/// 
template<typename Type1, typename ResultType>
class Callback1 : public ICallbackFunction
{
	/// Описание функция для вызова
	typedef std::function< ResultType ( const Type1& ) > Function;
public:
	/// @brief Конструктор
	///
	/// @param[in] name1 Имя переменной
	/// @param[in] function функия на вызов
	Callback1( const ValueName& name1, const Function& function ) 
		: function_( function ), nameValues_( name1 )
	{
	}
	/// @brief Деструктор
	///
	~Callback1()
	{
	}
public:
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @return Рузултат обработки функции
	rpc::Value operator()(const rpc::Value& value, const rpc::Value&, const rpc::info::IMessageInfoPtr&)
	{
		const auto parametr1 = rpc::value::Parametr<Type1>( std::get<0>( nameValues_ ), value );
		return rpc::value::Result<ResultType>( function_( parametr1 ) );
	}
private:
	///< Список имен переменных
	std::tuple<ValueName> nameValues_;
	///< Функция для вызова
	Function function_;	
};
//=====================================================================================================================
/// @brief Шаблон осщуествляет вызов зарегестрированной функции с двумя переменными
///
/// 
template<typename Type1, typename Type2, typename ResultType>
class Callback2 : public ICallbackFunction
{
	/// Описание функция для вызова
	typedef std::function< ResultType ( const Type1&, const Type2& ) > Function;
public:
	/// @brief Конструктор
	///
	/// @param[in] name1 Имя переменной
	/// @param[in] name2 Имя переменной
	/// @param[in] function функия на вызов
	Callback2( const ValueName& name1, const ValueName& name2, const Function& function ) 
		: function_( function ), nameValues_( name1, name2 )
	{
	}
	/// @brief Деструктор
	///
	~Callback2()
	{
	}
public:
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @return Рузултат обработки функции
	rpc::Value operator()(const rpc::Value& value, const rpc::Value&, const rpc::info::IMessageInfoPtr&)
	{
		const auto parametr1 = rpc::value::Parametr<Type1>( std::get<0>( nameValues_ ), value );
		const auto parametr2 = rpc::value::Parametr<Type2>( std::get<1>( nameValues_ ), value );
		return rpc::value::Result<ResultType>( function_( parametr1, parametr2 ) );
	}
private:
	///< Список имен переменных
	std::tuple<ValueName, ValueName> nameValues_;
	///< Функция для вызова
	Function function_;	
};
//=====================================================================================================================
/// @brief Шаблон осщуествляет вызов зарегестрированной функции с тремя переменными
///
/// 
template<typename Type1, typename Type2, typename Type3, typename ResultType>
class Callback3 : public ICallbackFunction
{
	/// Описание функция для вызова
	typedef std::function< ResultType ( const Type1&, const Type2&, const Type3& ) > Function;
public:
	/// @brief Конструктор
	///
	/// @param[in] name1 Имя переменной
	/// @param[in] name2 Имя переменной
	/// @param[in] name3 Имя переменной
	/// @param[in] function функия на вызов
	Callback3( const ValueName& name1, const ValueName& name2, const ValueName& name3, const Function& function ) 
		: function_( function ), nameValues_( name1, name2, name3 )
	{
	}
	/// @brief Деструктор
	///
	~Callback3()
	{
	}
public:
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @return Рузултат обработки функции
	rpc::Value operator()(const rpc::Value& value, const rpc::Value&, const rpc::info::IMessageInfoPtr&)
	{
		const auto parametr1 = rpc::value::Parametr<Type1>( std::get<0>( nameValues_ ), value );
		const auto parametr2 = rpc::value::Parametr<Type2>( std::get<1>( nameValues_ ), value );
		const auto parametr3 = rpc::value::Parametr<Type3>( std::get<2>( nameValues_ ), value );
		return rpc::value::Result<ResultType>( function_( parametr1, parametr2, parametr3 ) );
	}
private:
	///< Список имен переменных
	std::tuple<ValueName, ValueName, ValueName> nameValues_;
	///< Функция для вызова
	Function function_;	
};
//=====================================================================================================================
/// @brief Шаблон осщуествляет вызов зарегестрированной функции с 4 переменными
///
/// 
template<typename Type1, typename Type2, typename Type3,typename Type4, typename ResultType>
class Callback4 : public ICallbackFunction
{
	/// Описание функция для вызова
	typedef std::function< ResultType ( const Type1&, const Type2&, const Type3&, const Type4& ) > Function;
public:
	/// @brief Конструктор
	///
	/// @param[in] name1 Имя переменной
	/// @param[in] name2 Имя переменной
	/// @param[in] name3 Имя переменной
	/// @param[in] name4 Имя переменной
	/// @param[in] function функия на вызов
	Callback4( const ValueName& name1, const ValueName& name2, const ValueName& name3, const ValueName& name4, const Function& function ) 
		: function_( function ), nameValues_( name1, name2, name3, name4 )
	{
	}
	/// @brief Деструктор
	///
	~Callback4()
	{
	}
public:
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @return Рузултат обработки функции
	rpc::Value operator()(const rpc::Value& value, const rpc::Value&, const rpc::info::IMessageInfoPtr&)
	{
		const auto parametr1 = rpc::value::Parametr<Type1>( std::get<0>( nameValues_ ), value );
		const auto parametr2 = rpc::value::Parametr<Type2>( std::get<1>( nameValues_ ), value );
		const auto parametr3 = rpc::value::Parametr<Type3>( std::get<2>( nameValues_ ), value );
		const auto parametr4 = rpc::value::Parametr<Type4>( std::get<3>( nameValues_ ), value );
		return rpc::value::Result<ResultType>( function_( parametr1, parametr2, parametr3, parametr4 ) );
	}
private:
	///< Список имен переменных
	std::tuple<ValueName, ValueName, ValueName, ValueName> nameValues_;
	///< Функция для вызова
	Function function_;	
};
//=====================================================================================================================
/// @brief Шаблон осщуествляет вызов зарегестрированной функции с 5 переменными
///
/// 
template<typename Type1, typename Type2, typename Type3,typename Type4, typename Type5, typename ResultType>
class Callback5 : public ICallbackFunction
{
	/// Описание функция для вызова
	typedef std::function< ResultType ( const Type1&, const Type2&, const Type3&, const Type4&, const Type5& ) > Function;
public:
	/// @brief Конструктор
	///
	/// @param[in] name1 Имя переменной
	/// @param[in] name2 Имя переменной
	/// @param[in] name3 Имя переменной
	/// @param[in] name4 Имя переменной
	/// @param[in] name5 Имя переменной
	/// @param[in] function функия на вызов
	Callback5( const ValueName& name1, const ValueName& name2, const ValueName& name3, const ValueName& name4,  const ValueName& name5, const Function& function ) 
		: function_( function ), nameValues_( name1, name2, name3, name4, name5 )
	{
	}
	/// @brief Деструктор
	///
	~Callback5()
	{
	}
public:
	/// @brief Осуществляет вызов функции
	/// 
	/// @param[in] value Входные данные
	/// @return Рузултат обработки функции
	rpc::Value operator()(const rpc::Value& value, const rpc::Value&, const rpc::info::IMessageInfoPtr&)
	{
		const auto parametr1 = rpc::value::Parametr<Type1>( std::get<0>( nameValues_ ), value );
		const auto parametr2 = rpc::value::Parametr<Type2>( std::get<1>( nameValues_ ), value );
		const auto parametr3 = rpc::value::Parametr<Type3>( std::get<2>( nameValues_ ), value );
		const auto parametr4 = rpc::value::Parametr<Type4>( std::get<3>( nameValues_ ), value );
		const auto parametr5 = rpc::value::Parametr<Type5>( std::get<4>( nameValues_ ), value );
		return rpc::value::Result<ResultType>( function_( parametr1, parametr2, parametr3, parametr4, parametr5 ) );
	}
private:
	///< Список имен переменных
	std::tuple<ValueName, ValueName, ValueName, ValueName, ValueName> nameValues_;
	///< Функция для вызова
	Function function_;	
};
//=====================================================================================================================

//=====================================================================================================================
}
}
