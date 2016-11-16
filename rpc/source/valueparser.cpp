/// @file 
/// @brief Файл реализовывает частные случаипо разбору данных полученные по протоколу rpc
#include <rpc/include/precompiled.h>

namespace rpc
{
namespace value
{
//=====================================================================================================================
template<>
bool Parser<bool>::Get( const rpc::Value& value )
{
	if( !value.isBool() ){
		throw rpc::value::Exception( rpc::enums::ERROR_RPC_INVALID_PARAMS );
	}

	return value.asBool();
}
//=====================================================================================================================
template<>
int Parser<int>::Get( const rpc::Value& value )
{
	if( !value.isInt() ){
		throw rpc::value::Exception( rpc::enums::ERROR_RPC_INVALID_PARAMS );
	}

	return value.asInt();
}
//=====================================================================================================================
template<>
std::wstring Parser<std::wstring>::Get( const rpc::Value& value )
{
	if( !value.isString() ){
		throw rpc::value::Exception( rpc::enums::ERROR_RPC_INVALID_PARAMS );
	}

	return value.asString();
}
//=====================================================================================================================
template<>
double Parser<double>::Get( const rpc::Value& value )
{
	if( !value.isDouble() ){
		throw rpc::value::Exception( rpc::enums::ERROR_RPC_INVALID_PARAMS );
	}

	return value.asDouble();
}
//=====================================================================================================================
template<>
unsigned int Parser<unsigned int>::Get( const rpc::Value& value )
{
	if( !value.isUInt() ){
		throw rpc::value::Exception( rpc::enums::ERROR_RPC_INVALID_PARAMS );
	}

	return value.asUInt();
}
//=====================================================================================================================
template<>
rpc::value::MapData Parser<rpc::value::MapData>::Get(const rpc::Value& value)
{
	if (!value.isArray()){
		throw rpc::value::Exception(rpc::enums::ERROR_RPC_INVALID_PARAMS);
	}

	rpc::value::MapData result;

	for (const auto data : value())
	{
		result.insert(std::make_pair(data.first, rpc::Value(data.second).asString()));
	}

	return std::move(result);
}
//=====================================================================================================================
template<>
rpc::value::StructureData Parser<rpc::value::StructureData>::Get(const rpc::Value& value)
{
	rpc::value::StructureData structureData;

	for (const auto& data : value() )
	{
		rpc::Value valueData(data.second);

		if (!valueData.isString()){
			throw rpc::value::Exception(rpc::enums::ERROR_RPC_INVALID_PARAMS);
		}

		structureData.push_back(valueData.asString());
	}

	return std::move(structureData);
}
//=====================================================================================================================
template<>
rpc::value::IdList Parser<rpc::value::IdList>::Get(const rpc::Value& value)
{
	if (!value.isArray()){
		throw rpc::value::Exception(rpc::enums::ERROR_RPC_INVALID_PARAMS);
	}

	rpc::value::IdList idList;

	for (const auto& data : value())
	{
		rpc::Value valueData(data.second);

		if (!valueData.isInt()){
			throw rpc::value::Exception(rpc::enums::ERROR_RPC_INVALID_PARAMS);
		}

		idList.push_back(valueData.asInt());
	}

	return std::move(idList);
}
//=====================================================================================================================
template<>
rpc::Value Result<bool>( const bool& data )
{
	boost::property_tree::wptree result;
	result.put( KEY_RESPONSE_RESULT, data );

	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<int>( const int& data )
{
	boost::property_tree::wptree result;
	result.put( KEY_RESPONSE_RESULT, data );

	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<unsigned int>( const unsigned int& data )
{
	boost::property_tree::wptree result;
	result.put( KEY_RESPONSE_RESULT, data );

	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<double>( const double& data )
{
	boost::property_tree::wptree result;
	result.put( KEY_RESPONSE_RESULT, data );

	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<std::wstring>( const std::wstring& data )
{
	boost::property_tree::wptree result;
	result.put( KEY_RESPONSE_RESULT, data );

	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<rpc::value::StructureData>( const rpc::value::StructureData& data )
{
	boost::property_tree::wptree children;

	for( const auto& structure : data )
	{
		boost::property_tree::wptree value;
		value.put( L"", structure );
		children.push_back( std::make_pair( L"", value ) );
	}

	if( children.empty() )
	{
		children.put( KEY_RESPONSE_RESULT, L"null" );
		return rpc::Value( children );
	}

	boost::property_tree::wptree result;
	result.add_child( KEY_RESPONSE_RESULT, children );
	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<rpc::value::StructureDataList>( const rpc::value::StructureDataList& data )
{
	boost::property_tree::wptree children;

	for( const auto& structure : data )
	{
		const auto value = Result( structure );
		if( !value.isValid() ){
			continue;
		}
		
		children.push_back( std::make_pair( L"",  value[KEY_RESPONSE_RESULT]() ) );
	}

	if( children.empty() )
	{
		children.put( KEY_RESPONSE_RESULT, L"null" );
		return rpc::Value( children );
	}

	boost::property_tree::wptree result;
	result.add_child( KEY_RESPONSE_RESULT, children );
	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<rpc::value::MapData>( const rpc::value::MapData& data )
{
	boost::property_tree::wptree value;

	for( const auto& structure : data )
	{
		value.put( structure.first, structure.second );
	}

	boost::property_tree::wptree result;
	result.add_child( KEY_RESPONSE_RESULT, value );
	return rpc::Value( result );
}
//=====================================================================================================================
template<>
rpc::Value Result<rpc::value::MapDataList>( const rpc::value::MapDataList& data )
{
	boost::property_tree::wptree children;

	for( const auto& structure : data )
	{
		const auto value = Result( structure );
		if( !value.isValid() ){
			continue;
		}
		
		children.push_back( std::make_pair( L"",  value[KEY_RESPONSE_RESULT]() ) );
	}

	boost::property_tree::wptree result;
	result.add_child( KEY_RESPONSE_RESULT, children );
	return rpc::Value( result );
}
//=====================================================================================================================
}
}
