/// @file 
/// @brief Шаблон для создания указателя на функцию для обработки входящих данных по rpc
#pragma once

namespace rpc
{
namespace function
{
//=====================================================================================================================
template<typename Type1>
ICallbackPtr make_result_callback_ptr(const std::function< void(const Type1&, const rpc::info::IMessageInfoPtr& messageInfo) >& function,
			const std::function< void(const int code, const std::wstring& message, const rpc::info::IMessageInfoPtr& messageInfo) >& error)
{
	return std::make_shared<CallbackResult<Type1>>(function, error);
}
//=====================================================================================================================
template<typename ResultType>
ICallbackPtr make_callback_ptr( const std::function< ResultType ( void ) >& function )
{
	return std::make_shared<Callback0<ResultType>>( function );
}
//=====================================================================================================================
template<typename Type1, typename ResultType>
ICallbackPtr make_callback_ptr( const ICallback::ValueName& name1, 
								const std::function< ResultType ( const Type1& ) >& function )
{
	return std::make_shared<Callback1<Type1, ResultType>>( name1, function );
}
//=====================================================================================================================
template<typename Type1, typename Type2, typename ResultType>
ICallbackPtr make_callback_ptr( const ICallback::ValueName& name1, 
								const ICallback::ValueName& name2,
								const std::function< ResultType ( const Type1&, const Type2& ) >& function )
{
	return std::make_shared<Callback2<Type1, Type2, ResultType>>( name1, name2, function );
}
//=====================================================================================================================
template<typename Type1, typename Type2, typename Type3, typename ResultType>
ICallbackPtr make_callback_ptr( const ICallback::ValueName& name1, 
								const ICallback::ValueName& name2,
								const ICallback::ValueName& name3,
								const std::function< ResultType ( const Type1&, const Type2&, const Type3& ) >& function )
{
	return std::make_shared<Callback3<Type1, Type2, Type3, ResultType>>( name1, name2, name3, function );
}
//=====================================================================================================================
template<typename Type1, typename Type2, typename Type3, typename Type4, typename ResultType>
ICallbackPtr make_callback_ptr( const ICallback::ValueName& name1, 
								const ICallback::ValueName& name2,
								const ICallback::ValueName& name3,
								const ICallback::ValueName& name4,
								const std::function< ResultType ( const Type1&, const Type2&, const Type3&, const Type4& ) >& function )
{
	return std::make_shared<Callback4<Type1, Type2, Type3, Type4, ResultType>>( name1, name2, name3, name4, function );
}
//=====================================================================================================================
template<typename Type1, typename Type2, typename Type3, typename Type4, typename Type5, typename ResultType>
ICallbackPtr make_callback_ptr( const ICallback::ValueName& name1, 
								const ICallback::ValueName& name2,
								const ICallback::ValueName& name3,
								const ICallback::ValueName& name4,
								const ICallback::ValueName& name5,
								const std::function< ResultType ( const Type1&, const Type2&, const Type3&, const Type4&, const Type5& ) >& function )
{
	return std::make_shared<Callback5<Type1, Type2, Type3, Type4, Type5, ResultType>>( name1, name2, name3, name4, name5, function );
}
//=====================================================================================================================
}
}
