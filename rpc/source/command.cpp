
#include <rpc/include/precompiled.h>

using namespace rpc;
using namespace rpc::client;

const std::wstring& Command::GetFunctionName(void) const
{
	return functionName_;
}

const std::wstring& Command::GetCallbackFunctionName(void) const
{
	return callbackFunctionName_;
}

const rpc::Value& Command::GetValue(void) const
{
	return value_;
}

Command::Command(const std::wstring& functionName, const rpc::Value& value, const std::wstring& callbackFunctionName)
	: functionName_(functionName), value_(value), callbackFunctionName_(callbackFunctionName)
{

}

Command::~Command()
{

}
