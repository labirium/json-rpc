#pragma once

#include <boost/filesystem.hpp>

namespace rpc
{
namespace filesystem
{
/// @brief Возвращает путь до рабочей директории
///
boost::filesystem::path GetFolderWork(void);
/// @brief Возвращает путь до рабочей директории
///
boost::filesystem::path GetFolderClient(void);
}
}
