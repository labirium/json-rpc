#include <rpc/include/precompiled.h>

namespace rpc
{
//=====================================================================================================================
const std::wstring& Procedure::GetName( void ) const
{
	return name_;
}
//=====================================================================================================================
rpc::enums::ProcedureType Procedure::GetType( void ) const
{
	return callback_->Type();
}
//=====================================================================================================================
rpc::Value Procedure::operator()(const rpc::Value& input, const rpc::Value& error, const rpc::info::IConnectInfoPtr& connectInfo)
{
	return (*callback_)(input, error, std::make_shared<rpc::info::IMessageInfoPtr::element_type>(connectInfo, name_));
}
//=====================================================================================================================
Procedure::Procedure(const std::wstring& name, const rpc::function::ICallbackPtr& callback )
	: name_( name ), callback_( callback )
{
	assert( callback && "Invalid input data." );
}
//=====================================================================================================================
Procedure::~Procedure()
{
}
//=====================================================================================================================
}
