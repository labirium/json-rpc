/// @file 
/// @brief Файл содержит определение переменной
#pragma once

namespace rpc 
{
/// @brief Данные
///
class Value
{
public:
	/// @brief Конструктор
	///
	Value( void );
	/// @brief Конструктор
	///
	/// @param[in] ptree данные 
	explicit Value( const boost::property_tree::wptree& ptree );
	/// @brief Деструктор
	///
	~Value();
	
public:
	/// @brief Возвращает переменную по ключу. 
	/// После возвращения данных необходимо проверить на валидность
	/// 
	/// @param[in] key ключ
	/// @return Возвращает значение по ключу
	Value operator[]( const std::wstring& key ) const;
public:
	/// @brief Возвращает строку
	///
	std::wstring asString( void ) const;
	/// @brief Возвращает bool
	///
	bool asBool( void ) const;
	/// @brief Возвращает double
	///
	double asDouble( void ) const;
	/// @brief Возвращает uint
	///
	unsigned int asUInt( void ) const;
	/// @brief Возвращает int
	///
	int asInt( void ) const;
public:
	/// @brief Возвращает true если строка
	///
	bool isString( void ) const;
	/// @brief Возвращает true если bool
	///
	bool isBool( void ) const;
	/// @brief Возвращает true если double
	///
	bool isDouble( void ) const;
	/// @brief Возвращает true если uint
	///
	bool isUInt( void ) const;
	/// @brief Возвращает true если int
	///
	bool isInt( void ) const;
	/// @brief Возвращает true если null
	///
	bool isNull( void ) const;
	/// @brief Возвращает true если массиф
	///
	bool isArray( void ) const;
	/// @brief Возвращает true если объект
	///
	bool isObject( void ) const;
	/// @brief Возвращает true если существует ключ в текущем дереве
	///
	bool isMember( const std::wstring& key ) const;
	/// @brief Возвращает true если пустой
	///
	bool isEmpty( void ) const;
	/// @brief Возвращает true если валидный
	///
	bool isValid( void ) const;
public:
	/// @brief Возвращает дерево
	///
	const boost::property_tree::wptree& operator()( void ) const;
	/// @brief Возвращает количество данных в дереве
	///
	size_t size( void ) const;
private:
	///< Дерево данных
	std::shared_ptr<boost::property_tree::wptree> ptree_;
};

}


