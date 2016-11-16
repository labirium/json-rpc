//// @file 
/// @brief Файл содержит реализацию менеджера зарегестрированных функций
#include <rpc/include/precompiled.h>

namespace rpc
{
//=====================================================================================================================
void ProcedureManager::Add( const rpc::Procedure& procedure )
{
	procedureList_.insert( std::make_pair( procedure.GetName(), std::make_shared<rpc::Procedure>( procedure ) ) );
}
//=====================================================================================================================
rpc::ProcedurePtr ProcedureManager::operator[]( const std::wstring& name ) const
{
	//Поиск метода
	const auto iterator = procedureList_.find( name );
	if( iterator == procedureList_.end() ){
		return rpc::ProcedurePtr();
	}

	return iterator->second;
}
//=====================================================================================================================
ProcedureManager::ProcedureManager( void )
{
	
}
//=====================================================================================================================
ProcedureManager::~ProcedureManager()
{
}
//=====================================================================================================================
}
