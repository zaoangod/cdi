/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : MIT License
/*---------------------------------------------------------------------------*/

#include "../stdafx.h"
#include "OsInfoFx.h"
#include "UtilityFx.h"

typedef BOOL (WINAPI* FuncGetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
typedef BOOL (WINAPI* FuncGetNativeSystemInfo)(LPSYSTEM_INFO);
typedef BOOL (WINAPI* FuncIsWow64Process)(HANDLE hProcess,PBOOL Wow64Process);
typedef LONG (WINAPI* FuncRtlGetVersion)(POSVERSIONINFOEXW osvi);

BOOL GetVersionFx(POSVERSIONINFOEXW osvi)
{
	static FuncRtlGetVersion pRtlGetVersion = NULL;
	if (pRtlGetVersion == NULL)
	{
		HMODULE hModule = GetModuleHandle(_T("ntdll.dll"));
		if (hModule)
		{
			pRtlGetVersion = (FuncRtlGetVersion)GetProcAddress(hModule, "RtlGetVersion");
		}
	}

	if (pRtlGetVersion)
	{
		if (pRtlGetVersion(osvi) >= 0) // NT_SUCCESS(pRtlGetVersion(osvi) 
		{
			return TRUE;
		}
	}	
	return FALSE;
}

BOOL IsWindowsVersionOrGreaterFx(WORD wMajorVersion, WORD wMinorVersion, WORD wServicePackMajor)
{
	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
	UINT64 r, o;

	GetVersionFx(&osvi);
	o = ((UINT64)osvi.dwMajorVersion & 0xFFFF) << 32 | ((UINT64)osvi.dwMinorVersion & 0xFFFF) << 16 | (osvi.wServicePackMajor & 0xFFFF);
	r = ((UINT64)wMajorVersion & 0xFFFF) << 32 | ((UINT64)wMinorVersion & 0xFFFF) << 16 | (wServicePackMajor & 0xFFFF);
	return (o >= r);
}

BOOL IsWindowsBuildOrGreater(DWORD build)
{
	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
	GetVersionFx(&osvi);
	return (osvi.dwBuildNumber >= build);
}

BOOL IsX64()
{
	static BOOL b = -1;
	if (b == -1)
	{
		b = FALSE;
		SYSTEM_INFO si = { 0 };
		GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			b = TRUE;
		}
	}
	return b;
}

BOOL IsIa64()
{
	static BOOL b = -1;
	if (b == -1)
	{
		b = FALSE;
		SYSTEM_INFO si = { 0 };
		GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
		{
			b = TRUE;
		}
	}
	return b;
}

BOOL IsArm32()
{
	static BOOL b = -1;
	if (b == -1)
	{
		b = FALSE;
		SYSTEM_INFO si = { 0 };
		GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
		{
			b = TRUE;
		}
	}
	return b;
}

BOOL IsArm64()
{
	static BOOL b = -1;
	if (b == -1)
	{
		b = FALSE;
		SYSTEM_INFO si = { 0 };
		GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM64)
		{
			b = TRUE;
		}
	}
	return b;
}

BOOL IsWow64()
{
	static BOOL b = -1;
	if (b == -1)
	{
		b = FALSE;
		IsWow64Process(GetCurrentProcess(), &b);
	}
	return b;
}

BOOL IsIe556()
{
	return FALSE;
}

BOOL IsDotNet2()
{
	static BOOL b = -1;

	if (b == -1)
	{
		b = FALSE;
		DWORD type = REG_DWORD;
		ULONG size = sizeof(DWORD);
		HKEY  hKey = NULL;
		DWORD buf = 0;

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v2.0.50727"), 0, KEY_READ, &hKey) == ERROR_SUCCESS
		||  RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Wow6432Node\\Microsoft\\NET Framework Setup\\NDP\\v2.0.50727"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, _T("Install"), NULL, &type, (LPBYTE)&buf, &size) == ERROR_SUCCESS)
			{
				if (buf == 1)
				{
					b = TRUE;
				}
			}
			RegCloseKey(hKey);
		}
	}

	return (BOOL)b;
}

BOOL IsDotNet4()
{
	static BOOL b = -1;

	if (b == -1)
	{
		b = FALSE;
		DWORD type = REG_DWORD;
		ULONG size = sizeof(DWORD);
		HKEY  hKey = NULL;
		DWORD buf = 0;

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Client"), 0, KEY_READ, &hKey) == ERROR_SUCCESS
		||  RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full"), 0, KEY_READ, &hKey) == ERROR_SUCCESS
		||  RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\WOW6432Node\\Microsoft\\NET Framework Setup\\NDP\\v4\\Client"), 0, KEY_READ, &hKey) == ERROR_SUCCESS
		||  RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\WOW6432Node\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full"), 0, KEY_READ, &hKey) == ERROR_SUCCESS
			)
		{
			if (RegQueryValueEx(hKey, _T("Install"), NULL, &type, (LPBYTE)&buf, &size) == ERROR_SUCCESS)
			{
				if (buf == 1)
				{
					b = TRUE;
				}
			}
			RegCloseKey(hKey);
		}
	}

	return b;
}

BOOL IsDotNet48()
{
	static BOOL b = -1;

	if (b == -1)
	{
		b = FALSE;
		DWORD type = REG_DWORD;
		ULONG size = sizeof(DWORD);
		HKEY  hKey = NULL;
		DWORD buf = 0;

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Client"), 0, KEY_READ, &hKey) == ERROR_SUCCESS
		||  RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\NET Framework Setup\\NDP\\v4\\Full"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, _T("Install"), NULL, &type, (LPBYTE)&buf, &size) == ERROR_SUCCESS)
			{
				if (buf == 1)
				{
					if (RegQueryValueEx(hKey, _T("Release"), NULL, &type, (LPBYTE)&buf, &size) == ERROR_SUCCESS)
					{
						if (buf >= 528040)
						{
							b = TRUE;
						}
					}
				}
			}
			RegCloseKey(hKey);
		}
	}

	return b;
}

// Win10+ always returns TRUE
BOOL IsNT6orLater()
{
	return TRUE;
}

// Win10+ always returns TRUE
BOOL IsWin8orLater()
{
	return TRUE;
}

// Win10+ always returns TRUE
BOOL IsWin81orLater()
{
	return TRUE;
}

// Win10 build 17763+ always returns TRUE
BOOL IsDarkModeSupport()
{
	return TRUE;
}

BOOL IsRunningOnWine()
{
	HMODULE hModule = LoadLibraryA("ntdll.dll");
	if (hModule)
	{
		void* pWineGetVersion = (void*)GetProcAddress(hModule, "wine_get_version");
		FreeLibrary(hModule);
		return pWineGetVersion != NULL;
	}
	return FALSE;
}

void GetOsName(CString& osFullName, CString& name, CString& version, CString& architecture)
{
	CString osName, osType, osCsd, osVersion, osBuild, osArchitecture, osDisplayVersion;
	CString osNameWmi;
	CString cstr;

	OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
	GetVersionFx((OSVERSIONINFOEXW*)&osvi);

	GetOsNameWmi(osNameWmi);

	// Windows 11/Server 2025
	CString osNameWmiBackup = osNameWmi;
	CString osNameWmiMajorVersion = osNameWmi;
	osNameWmiMajorVersion.Replace(_T("(R)"), _T(""));
	osNameWmiMajorVersion.Replace(_T("Microsoft "), _T(""));
	osNameWmiMajorVersion.Replace(_T("Windows "), _T(""));
	osNameWmiMajorVersion.Replace(_T("Server "), _T(""));

	int majorVersion = _ttoi(osNameWmiMajorVersion);

	if ((3 <= majorVersion && majorVersion <= 11) || (2000 <= majorVersion && majorVersion <= 2025))
	{
		osNameWmi = _T("");
	}

	if (osNameWmi.IsEmpty())
	{
		switch (osvi.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			if ((osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 4)
				|| (osvi.dwMajorVersion >= 10 && osvi.dwMinorVersion >= 0))
			{
				if (osvi.wProductType != VER_NT_WORKSTATION)
				{
					if (osvi.dwBuildNumber >= 26040)
					{
						osName = _T("Windows Server 2025");
					}
					else if (osvi.dwBuildNumber >= 20344)
					{
						osName = _T("Windows Server 2022");
					}
					else if (osvi.dwBuildNumber >= 17763)
					{
						osName = _T("Windows Server 2019");
					}
					else
					{
						osName = _T("Windows Server 2016");
					}
				}
				else
				{
					if (osvi.dwBuildNumber >= 21996)
					{
						osName = _T("Windows 11");
					}
					else
					{
						osName = _T("Windows 10");
					}
				}
			}
			else
			{
				osName.Format(_T("Windows NT %d.%d"), osvi.dwMajorVersion, osvi.dwMinorVersion);
			}

			if (osvi.dwMajorVersion >= 6)
			{
				DWORD productType = 0;
				GetProductInfo(osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &productType);

				switch (productType)
				{
				case PRODUCT_UNLICENSED:
					osType = _T("Unlicensed");
					break;
				case PRODUCT_CORE_ARM:
				case PRODUCT_CORE_COUNTRYSPECIFIC:
				case PRODUCT_CORE_SINGLELANGUAGE:
				case PRODUCT_CORE:
					osType = L"Home";
					break;
				case PRODUCT_CORE_N:
					osType = L"Home N";
					break;
				case PRODUCT_EDUCATION:
					osType = L"Education";
					break;
				case PRODUCT_EDUCATION_N:
					osType = L"Education N";
					break;
				case PRODUCT_BUSINESS:
					osType = _T("Business");
					break;
				case PRODUCT_BUSINESS_N:
					osType = _T("Business N");
					break;
				case PRODUCT_CLUSTER_SERVER:
					osType = _T("Cluster Server");
					break;
				case PRODUCT_DATACENTER_SERVER:
					osType = _T("Datacenter");
					break;
				case PRODUCT_DATACENTER_SERVER_CORE:
					osType = _T("Datacenter");
					break;
				case PRODUCT_ENTERPRISE:
					osType = _T("Enterprise");
					break;
				case PRODUCT_ENTERPRISE_N:
					osType = _T("Enterprise N");
					break;
				case PRODUCT_ENTERPRISE_G:
					osType = _T("Enterprise G");
					break;
				case PRODUCT_ENTERPRISE_SERVER:
					osType = _T("Enterprise");
					break;
				case PRODUCT_ENTERPRISE_SERVER_CORE:
					osType = _T("Enterprise");
					break;
				case PRODUCT_ENTERPRISE_SERVER_IA64:
					osType = _T("Datacenter Enterprise for Itanium-based Systems");
					break;
				case PRODUCT_HOME_BASIC:
					osType = _T("Home Basic");
					break;
				case PRODUCT_HOME_BASIC_N:
					osType = _T("Home Basic N");
					break;
				case PRODUCT_HOME_PREMIUM:
					osType = _T("Home Premium");
					break;
				case PRODUCT_HOME_PREMIUM_N:
					osType = _T("Home Premium N");
					break;
				case PRODUCT_HOME_SERVER:
					osType = _T("Home Server");
					break;
				case PRODUCT_SERVER_FOR_SMALLBUSINESS:
					osType = _T("Server for Small Business");
					break;
				case PRODUCT_SMALLBUSINESS_SERVER:
					osType = _T("Small Business Server");
					break;
				case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
					osType = _T("Small Business Server Premium");
					break;
				case PRODUCT_STANDARD_SERVER:
					osType = _T("Server Standard");
					break;
				case PRODUCT_STANDARD_SERVER_CORE:
					osType = _T("Server Standard");
					break;
				case PRODUCT_STARTER:
					osType = _T("Starter");
					break;
				case PRODUCT_STARTER_N:
					osType = _T("Starter N");
					break;
				case  PRODUCT_STARTER_E:
					osType = _T("Starter E");
					break;
				case PRODUCT_STORAGE_ENTERPRISE_SERVER:
					osType = _T("Storage Server Enterprise");
					break;
				case PRODUCT_STORAGE_EXPRESS_SERVER:
					osType = _T("Storage Server Express");
					break;
				case PRODUCT_STORAGE_STANDARD_SERVER:
					osType = _T("Storage Server Standard");
					break;
				case PRODUCT_STORAGE_WORKGROUP_SERVER:
					osType = _T("Storage Server Workgroup");
					break;
				case PRODUCT_ULTIMATE:
					osType = _T("Ultimate");
					break;
				case PRODUCT_ULTIMATE_N:
					osType = _T("Ultimate N");
					break;
				case PRODUCT_WEB_SERVER:
					osType = _T("Web Server");
					break;
				case PRODUCT_PROFESSIONAL:
					osType = _T("Pro");
					break;
				case PRODUCT_PROFESSIONAL_N:
					osType = _T("Pro N");
					break;
				case PRODUCT_PRO_WORKSTATION:
				case PRODUCT_PROFESSIONAL_WORKSTATION:
					osType = _T("Pro for Workstation");
					break;
				case PRODUCT_PRO_WORKSTATION_N:
				case PRODUCT_PROFESSIONAL_WORKSTATION_N:
					osType = _T("Pro for Workstation N");
					break;
				case PRODUCT_PRO_FOR_EDUCATION:
					osType = _T("Pro for Education");
					break;
				case PRODUCT_PRO_FOR_EDUCATION_N:
					osType = _T("Pro for Education N");
					break;
				case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDL:
				case PRODUCT_ESSENTIALBUSINESS_SERVER_MGMTSVC:
				case PRODUCT_ESSENTIALBUSINESS_SERVER_ADDLSVC:
					osType = _T("Essentials Server");
					break;
				}
			}
		}
	}

	osCsd = osvi.szCSDVersion;
	osCsd.Replace(_T("Service Pack "), _T("SP"));

	if (osvi.dwMajorVersion >= 10)
	{
		DWORD value = 0;
		DWORD type = REG_SZ;
		TCHAR str[256];
		ULONG size = 256 * sizeof(TCHAR);
		HKEY  hKey = NULL;

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueEx(hKey, _T("DisplayVersion"), NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				osDisplayVersion = str;
			}

			if (osDisplayVersion.IsEmpty() && RegQueryValueEx(hKey, _T("ReleaseId"), NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				osDisplayVersion = str;
			}
		}
	}

	osVersion.Format(_T("%d.%d"), osvi.dwMajorVersion, osvi.dwMinorVersion);
	osBuild.Format(_T("%d"), osvi.dwBuildNumber);

	if(IsX64())
	{
		osArchitecture = _T("x64");
	}
	else if (IsArm32())
	{
		osArchitecture = _T("ARM32");
	}
	else if (IsArm64())
	{
		osArchitecture = _T("ARM64");
	}
	else if(IsIa64())
	{
		osArchitecture = _T("IA64");
	}
	else
	{
		osArchitecture = _T("x86");
	}

	// for Unknown Edition
	if (osType.IsEmpty())
	{
		osNameWmi = osNameWmiBackup;
	}

	if (!osNameWmi.IsEmpty())
	{
		if (!osDisplayVersion.IsEmpty())
		{
			osFullName.Format(_T("%s %s [%s Build %s] (%s)"), (LPCTSTR)osNameWmi, (LPCTSTR)osDisplayVersion, (LPCTSTR)osVersion, (LPCTSTR)osBuild, (LPCTSTR)osArchitecture);
			name.Format(_T("%s %s"), (LPCTSTR)osNameWmi, (LPCTSTR)osDisplayVersion);
			version.Format(_T("%s Build %s"), (LPCTSTR)osVersion, (LPCTSTR)osBuild);
			architecture = osArchitecture;			
		}
		else if (!osCsd.IsEmpty())
		{
			osFullName.Format(_T("%s %s [%s Build %s] (%s)"), (LPCTSTR)osNameWmi, (LPCTSTR)osCsd, (LPCTSTR)osVersion, (LPCTSTR)osBuild, (LPCTSTR)osArchitecture);
			name.Format(_T("%s %s"), (LPCTSTR)osNameWmi, (LPCTSTR)osCsd);
			version.Format(_T("%s Build %s"), (LPCTSTR)osVersion, (LPCTSTR)osBuild);
			architecture = osArchitecture;
		}
		else
		{
			osFullName.Format(_T("%s [%s Build %s] (%s)"), (LPCTSTR)osNameWmi, (LPCTSTR)osVersion, (LPCTSTR)osBuild, (LPCTSTR)osArchitecture);
			name.Format(_T("%s"), (LPCTSTR)osNameWmi);
			version.Format(_T("%s Build %s"), (LPCTSTR)osVersion, (LPCTSTR)osBuild);
			architecture = osArchitecture;
		}
	}
	else
	{
		if (!osDisplayVersion.IsEmpty())
		{
			osFullName.Format(_T("%s %s %s [%s Build %s] (%s)"), (LPCTSTR)osName, (LPCTSTR)osType, (LPCTSTR)osDisplayVersion, (LPCTSTR)osVersion, (LPCTSTR)osBuild, (LPCTSTR)osArchitecture);
			name.Format(_T("%s %s %s"), (LPCTSTR)osName, (LPCTSTR)osType, (LPCTSTR)osDisplayVersion);
			version.Format(_T("%s Build %s"), (LPCTSTR)osVersion, (LPCTSTR)osBuild);
			architecture = osArchitecture;
		}
		else if (!osCsd.IsEmpty())
		{
			osFullName.Format(_T("%s %s %s [%s Build %s] (%s)"), (LPCTSTR)osName, (LPCTSTR)osType, (LPCTSTR)osCsd, (LPCTSTR)osVersion, (LPCTSTR)osBuild, (LPCTSTR)osArchitecture);
			name.Format(_T("%s %s %s"), (LPCTSTR)osName, (LPCTSTR)osType, (LPCTSTR)osCsd);
			version.Format(_T("%s Build %s"), (LPCTSTR)osVersion, (LPCTSTR)osBuild);
			architecture = osArchitecture;
		}
		else if (!osType.IsEmpty())
		{
			osFullName.Format(_T("%s %s [%s Build %s] (%s)"), (LPCTSTR)osName, (LPCTSTR)osType, (LPCTSTR)osVersion, (LPCTSTR)osBuild, (LPCTSTR)osArchitecture);
			name.Format(_T("%s %s"), (LPCTSTR)osName, (LPCTSTR)osType);
			version.Format(_T("%s Build %s"), (LPCTSTR)osVersion, (LPCTSTR)osBuild);
			architecture = osArchitecture;
		}
		else
		{
			osFullName.Format(_T("%s [%s Build %s] (%s)"), (LPCTSTR)osName, (LPCTSTR)osVersion, (LPCTSTR)osBuild, (LPCTSTR)osArchitecture);
			name.Format(_T("%s"), (LPCTSTR)osName);
			version.Format(_T("%s Build %s"), (LPCTSTR)osVersion, (LPCTSTR)osBuild);
			architecture = osArchitecture;
		}	
	}

	osFullName.Replace(_T("  "), _T(" "));
}

//------------------------------------------------
// Get OS Information by WMI
//------------------------------------------------

#pragma warning(disable : 26812)

#include <comdef.h>
#include <comutil.h>
#include <wbemcli.h>
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "wbemuuid.lib")

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#ifndef safeCloseHandle
#define safeCloseHandle(h) { if( h != NULL ) { ::CloseHandle(h); h = NULL; } }
#endif
#ifndef safeVirtualFree
#define safeVirtualFree(h,b,c) { if( h != NULL ) { ::VirtualFree(h, b, c); h = NULL; } }
#endif

void GetOsNameWmi(CString& osName)
{
	CString query = _T("Select * from Win32_OperatingSystem");
	
	IWbemLocator*			pIWbemLocator = NULL;
	IWbemServices*			pIWbemServices = NULL;
	IEnumWbemClassObject*	pEnumCOMDevs = NULL;
	IWbemClassObject*		pCOMDev = NULL;
	ULONG					uReturned = 0;
	BOOL					flag = FALSE;

	try
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&pIWbemLocator)))
		{
			long securityFlag = WBEM_FLAG_CONNECT_USE_MAX_WAIT;
			if (SUCCEEDED(pIWbemLocator->ConnectServer(_bstr_t(_T("root\\cimv2")),
				NULL, NULL, 0L, securityFlag, NULL, NULL, &pIWbemServices)))
			{
				if (SUCCEEDED(CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
					NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
				{
					if (SUCCEEDED(pIWbemServices->ExecQuery(_bstr_t(_T("WQL")),
						_bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumCOMDevs)))
					{
						while (pEnumCOMDevs && SUCCEEDED(pEnumCOMDevs->Next(10000, 1, &pCOMDev, &uReturned)) && uReturned == 1)
						{
							CString name;
							VARIANT pVal;
							VariantInit(&pVal);

							if (pCOMDev->Get(L"Caption", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								name = pVal.bstrVal;
								VariantClear(&pVal);
								name.Replace(_T("\x2122"), _T(""));
								name.Replace(_T("\x00AE"), _T(""));
								name.Replace(_T("(R)"), _T(""));
								name.Replace(_T("Microsoft "), _T(""));
								
								name.Trim();
							}
							VariantInit(&pVal);

							osName = name;
						}
					}
				}
			}
		}
	}
	catch (...)
	{
		
	}

	SAFE_RELEASE(pCOMDev);
	SAFE_RELEASE(pEnumCOMDevs);
	SAFE_RELEASE(pIWbemServices);
	SAFE_RELEASE(pIWbemLocator);
}
