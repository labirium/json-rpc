/// @file 
/// @brief Файл содержит объявление преобразования типа 
#pragma once

namespace http
{
namespace server 
{
namespace mime_types
{
/// @brief Осуществляет конвертацию типа для MIMO 
///
/// @param[in] extension Значение на конвертацию
/// @return Возвращает результат конвертации
std::string ExtensionToType( const std::string& extension );
} 
} 
} 

