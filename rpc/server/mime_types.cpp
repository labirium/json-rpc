/// @file 
/// @brief Файл содержит реализацию преобразования типа 
#include <rpc/include/precompiled.h>

namespace
{
/// Значение по умолчанию
const char* const DEFAULT = "text/plain";
}

namespace
{
/// Список для хранения описания типов  
typedef std::map<std::string, std::string> MappingList;
//=====================================================================================================================
MappingList GetMappingList( void )
{
	MappingList mappingList;
	mappingList.insert( std::make_pair( "gif", "image/gif" ) );
	mappingList.insert( std::make_pair( "htm", "text/html" ) );
	mappingList.insert( std::make_pair( "html", "text/html" ) );
	mappingList.insert( std::make_pair( "jpg", "image/jpeg" ) );
	mappingList.insert( std::make_pair( "png", "image/png" ) );

	return mappingList;
}
//=====================================================================================================================
}

namespace http 
{
namespace server 
{
namespace mime_types 
{
//=====================================================================================================================
std::string ExtensionToType( const std::string& extension )
{
	//Список ошибок
	static MappingList mappingList;

	//Иницилизация списка
	static std::once_flag onceFlag;
	std::call_once( onceFlag, []()
	{
		mappingList = GetMappingList();
	} );

	//Осуществляем поиск и возврат результата поиска
	const auto iterator = mappingList.find( extension );
	return iterator != mappingList.end() ? iterator->second : std::string( DEFAULT );
}
//=====================================================================================================================
} 
} 
} 
