#include <rpc/include/precompiled.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#ifndef WIN32_NO_STATUS
#define WIN32_NO_STATUS
#endif

// Windows Header Files:
#include <windows.h>
//#include <rpc.h>
#include <ntstatus.h>
//#include <shlobj.h>
#pragma comment(lib,"shlwapi.lib")

#include <boost/filesystem.hpp>

#define CSIDL_COMMON_APPDATA            0x0023        // All Users\Application Data


EXTERN_C DECLSPEC_IMPORT HRESULT STDAPICALLTYPE SHGetFolderPathW(_Reserved_ HWND hwnd, _In_ int csidl, _In_opt_ HANDLE hToken, _In_ DWORD dwFlags, _Out_writes_(MAX_PATH) LPWSTR pszPath);

namespace rpc
{
namespace filesystem
{
boost::filesystem::path GetFolderWork(void)
{
	wchar_t szPath[MAX_PATH];
	// Get path for each computer, non-user specific and non-roaming data.
	if (FAILED(SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
	{
		throw std::runtime_error("Invalid get program data path");
	}

	return  boost::filesystem::path(szPath) / "rpc";
}

boost::filesystem::path GetFolderClient(void)
{
	wchar_t szPath[MAX_PATH];
	// Get path for each computer, non-user specific and non-roaming data.
	if (FAILED(SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, 0, szPath)))
	{
		throw std::runtime_error("Invalid get program data path");
	}

	return  boost::filesystem::path(szPath) / "rpc";
}
}
}