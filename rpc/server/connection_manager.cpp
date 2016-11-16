#include <rpc/include/precompiled.h>

namespace http 
{
namespace server 
{

void ConnectionManager::Start( const ConnectionPtr& connection )
{
  connections_.insert( connection );
  connection->Start();
}

void ConnectionManager::Stop( const ConnectionPtr& connection )
{
  connections_.erase( connection );
  connection->Stop();
}

void ConnectionManager::StopAll()
{
  for (auto connection: connections_ ){
	  connection->Stop();
  }

  connections_.clear();
}

ConnectionManager::ConnectionManager()
{
}

ConnectionManager::~ConnectionManager()
{
}

} 
} 
