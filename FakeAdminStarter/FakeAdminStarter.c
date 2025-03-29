// Project1.cpp : 定义应用程序的入口点。
//

#include "pch.h"

static int Main()
{
	HANDLE token;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &token))
	{
		return GetLastError();
	}
	// process token will be replaced by restricted token
	if (!CreateRestrictedToken(token, LUA_TOKEN, 0, 0, 0, 0, 0, 0, &token))
	{
		return GetLastError();
	}
	PSID medium;
	if (!ConvertStringSidToSidW(L"S-1-16-0x2000", &medium))
	{
		return GetLastError();
	}
	TOKEN_MANDATORY_LABEL setmedium = { {medium,SE_GROUP_INTEGRITY} };
	if (!SetTokenInformation(token, TokenIntegrityLevel, &setmedium, sizeof(TOKEN_MANDATORY_LABEL)))
	{
		return GetLastError();
	}
	PSECURITY_DESCRIPTOR  sa;
	if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:AI(A;;GA;;;SY)(A;;GA;;;BA)(A;;GA;;;BU)S:AI", SDDL_REVISION_1, &sa, 0))
	{
		return GetLastError();
	}
	BOOL hasdac;
	BOOL dacdef;
	PACL acl;
	if (!GetSecurityDescriptorDacl(sa, &hasdac, &acl, &dacdef))
	{
		return GetLastError();
	}
	if (!hasdac)
	{
		acl = 0;
	}
	TOKEN_DEFAULT_DACL dac = { acl };
	if (!SetTokenInformation(token, TokenDefaultDacl, &dac, 100))
	{
		return GetLastError();
	}
	STARTUPINFOW si = {
	sizeof(STARTUPINFOW),//	DWORD   cb;
	0,//LPWSTR  lpReserved;
	0,//LPWSTR  lpDesktop;
	0,//LPWSTR  lpTitle;
	0,//DWORD   dwX;
	0,//DWORD   dwY;
	0,//DWORD   dwXSize;
	0,//DWORD   dwYSize;
	0,//DWORD   dwXCountChars;
	0,//DWORD   dwYCountChars;
	0,//DWORD   dwFillAttribute;
	0,//DWORD   dwFlags;
	0,//WORD    wShowWindow;
	0,//WORD    cbReserved2;
	0,//LPBYTE  lpReserved2;
	0,//HANDLE  hStdInput;
	0,//HANDLE  hStdOutput;
	0//HANDLE  hStdError; 
	};
	PROCESS_INFORMATION pi;
	wchar_t* cmdl = _get_wide_winmain_command_line();
	wchar_t cmdld[] = L"cmd";
	if (!CreateProcessAsUserW(token, 0, cmdl[0]?cmdl:cmdld, 0, 0, 0, 0, 0, 0, &si, &pi))
	{
		return GetLastError();
	}
	if (!CloseHandle(pi.hThread))
	{
		return GetLastError();
	}
	if (!CloseHandle(pi.hProcess))
	{
		return GetLastError();
	}
	return 0;
}
void Startup()
{
	ExitProcess(Main());
}

