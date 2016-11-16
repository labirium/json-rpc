
#include <rpc/include/precompiled.h>

using namespace rpc;
using namespace rpc::info;

const std::string& IConnectInfo::GetPort(void) const
{
	return port_;
}

const std::string& IConnectInfo::GetHost(void) const
{
	return host_;
}

IConnectInfo::IConnectInfo(const std::string& host, const std::string& port) : host_(host), port_(port)
{

}

IConnectInfo::~IConnectInfo()
{

}

const IConnectInfoPtr& IMessageInfo::GetIConnectInfo(void) const
{
	return connectInfo_;
}

const std::wstring& IMessageInfo::GetId(void) const
{
	return id_;
}

IMessageInfo::IMessageInfo(const IConnectInfoPtr& connectInfo, const std::wstring& id) : connectInfo_(connectInfo), id_(id)
{

}

IMessageInfo::~IMessageInfo()
{

}
