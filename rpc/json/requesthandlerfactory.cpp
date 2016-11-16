/// @file 
/// @brief Файл содержит реализацию функции для получения интерфейса для обработки команд rpc

#include <rpc/include/precompiled.h>

namespace rpc
{
namespace factory
{
//=====================================================================================================================
IProtocolPtr CreateProtocol( const rpc::ProcedureManager& procedureManager, const rpc::enums::ServerVersion version )
{
    switch( version ) 
	{
        case rpc::enums::RPC_SERVER_V2:
			return std::make_shared<rpc::RpcProtocolServerV2>( procedureManager );
		case rpc::enums::RPC_SERVER_V1:
        case rpc::enums::RPC_SERVER_V12:
			throw std::runtime_error("Invalid support protocol.");
    }

    return IProtocolPtr();
}
//=====================================================================================================================
template<>
IProtocolPtr CreateProtocol<rpc::enums::RPC_SERVER_V2>( const rpc::ProcedureManager& procedureManager )
{
	return std::make_shared<rpc::RpcProtocolServerV2>( procedureManager );
}
//=====================================================================================================================
}
}
