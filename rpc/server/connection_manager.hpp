#pragma once

namespace http 
{
namespace server 
{

/// Manages open connections so that they may be cleanly stopped when the Server
/// needs to shut down.
class ConnectionManager
{
public:

	/// Construct a connection manager.
  ConnectionManager( void );

  ~ConnectionManager();

  /// Add the specified connection to the manager and start it.
  void Start( const ConnectionPtr& connection );

  /// Stop the specified connection.
  void Stop( const ConnectionPtr& connection );

  /// Stop all connections.
  void StopAll();

private:
  /// The managed connections.
  std::set<ConnectionPtr> connections_;
};

} // namespace server
} // namespace http
