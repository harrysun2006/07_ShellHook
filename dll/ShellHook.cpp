// ShellHook.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "ShellHook.h"
#include <fstream>
#include <map>

#undef USE_SHELLHOOK_MAP
#undef LOG_SHELLHOOK
#define SHELLHOOK_ENTRY_MAX			128
#define SHELLHOOK_ENTRY_NOTFOUND	-1
#define SHELLHOOK_MUTEX				"SHELL_HOOK_MUTEX"

using namespace std;

typedef struct SHELLHOOK_ENTRY {
//	HWND hAncestorWnd;
	HWND hWnd;
	UINT uMsg;
} SHELLHOOK_ENTRY;

typedef SHELLHOOK_ENTRY* LPSHELLHOOK_ENTRY;

#ifdef USE_SHELLHOOK_MAP
typedef map<UINT, SHELLHOOK_ENTRY> SHELLHOOK_MAP;
typedef pair<UINT, SHELLHOOK_ENTRY> SHELLHOOK_PAIR;
#endif

#pragma data_seg("mydata")
HINSTANCE g_hDllInstance = NULL;
#ifdef USE_SHELLHOOK_MAP
// #pragma声明一个数据段, 在此范围内声明的变量在初始化后将被指派到该数据段;
// 如未初始化, 变量将被分配到缺省数据段, 而#pragma不起作用。
// map等C++对象在共享数据段中初始化使用有问题(大小不确定)
SHELLHOOK_MAP g_hookMap;
#else
SHELLHOOK_ENTRY g_hookEntries[SHELLHOOK_ENTRY_MAX] = {0};
int g_Index = 0, g_Size = 0;
#endif
HHOOK g_hHook = NULL;
#ifdef LOG_SHELLHOOK
ofstream g_fLog("E:\\hook.log", ios::out|ios::app);
#endif
#pragma data_seg()
#pragma comment(linker, "/SECTION:mydata,RWS")

LRESULT CALLBACK ShellHookProcEx(int nCode, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH:
			g_hDllInstance = hInstance;
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
#ifdef LOG_SHELLHOOK
			g_fLog.close();
#endif
			break;
	}
	return TRUE;
}

#if WINVER < 0x0500
#define     GA_MIC          1
#define     GA_PARENT       1
#define     GA_ROOT         2
#define     GA_ROOTOWNER    3
#define     GA_MAC          4
HWND GetAncestor(HWND hwnd, UINT gaFlags)
{
	if (!IsWindow(hwnd)) return NULL;
	HWND hParentWnd = NULL;
	while(hwnd) {
		hParentWnd = hwnd;
		hwnd = GetParent(hwnd);
	}
	return hParentWnd;
}
#endif

#ifndef USE_SHELLHOOK_MAP
int AddShellHookEntry(HWND hWnd)
{
	int index = SHELLHOOK_ENTRY_NOTFOUND;
	HANDLE hMutex = CreateMutex(NULL, TRUE, SHELLHOOK_MUTEX);
	for (int i = 0; i < g_Size; i++)
	{
		if (g_hookEntries[i].hWnd == hWnd)
		{
			index = i;
			break;
		}
	}
	if (index < 0 && g_Index >= 0) 
	{
		if (g_Index >= SHELLHOOK_ENTRY_MAX) 
		{
			index = g_Index = -1;
		}
		else
		{
			g_Size++;
			index = g_Index++;
		}
	}
	ReleaseMutex(hMutex);
	return index;
}

BOOL RemoveShellHookEntry(HWND hWnd)
{
	int index = SHELLHOOK_ENTRY_NOTFOUND;
	HANDLE hMutex = CreateMutex(NULL, TRUE, SHELLHOOK_MUTEX);
	for (int i = 0; i < g_Size; i++)
	{
		if (g_hookEntries[i].hWnd == hWnd)
		{
			index = i;
			break;
		}
	}
	if (index >= 0)
	{
		g_Size--;
		g_Index--;
		g_hookEntries[index].hWnd = g_hookEntries[g_Size].hWnd;
		g_hookEntries[index].uMsg = g_hookEntries[g_Size].uMsg;
		g_hookEntries[g_Size].hWnd = 0;	
	}
	ReleaseMutex(hMutex);
	return (index >= 0);
}
#endif

BOOL WINAPI InstallShellHook(HWND hWnd, UINT uMsg)  
{
#ifdef USE_SHELLHOOK_MAP
	return FALSE;
#else
	return InstallShellHookEx(hWnd, uMsg);
#endif
}

BOOL WINAPI InstallShellHookEx(HWND hWnd, UINT uMsg)  
{
	if (g_hDllInstance == NULL) return FALSE;
	BOOL hr = FALSE;
#ifdef USE_SHELLHOOK_MAP
	HANDLE hMutex = CreateMutex(NULL, TRUE, SHELLHOOK_MUTEX);
	SHELLHOOK_MAP::iterator it = g_hookMap.find(UINT(hWnd));
	if (it == g_hookMap.end())
	{
		SHELLHOOK_ENTRY he;
		he.hWnd = hWnd;
		he.uMsg = uMsg;
		g_hookMap.insert(SHELLHOOK_PAIR(UINT(hWnd), he));
		hr = TRUE;
	}
	ReleaseMutex(hMutex);
#ifdef LOG_SHELLHOOK
	g_fLog << "Total " << g_hookMap.size() << " shell hook entries!" << endl;
#endif
#else
	if (g_Index < 0) return FALSE;
	int index = AddShellHookEntry(hWnd);
	if (index < 0 || index >= SHELLHOOK_ENTRY_MAX) return FALSE;
	if (g_hookEntries[index].hWnd) return FALSE;
	g_hookEntries[index].hWnd = hWnd;
	g_hookEntries[index].uMsg = uMsg;
	hr = TRUE;
#ifdef LOG_SHELLHOOK
	g_fLog << "Total " << g_Size << " shell hook entries!" << endl;
#endif
#endif
	if (g_hHook == NULL)
	{
		HHOOK hhook = NULL;
		hhook = SetWindowsHookEx(WH_SHELL, ShellHookProcEx, g_hDllInstance, 0);
		InterlockedExchange((PLONG)&g_hHook, (LONG)hhook);
#ifdef LOG_SHELLHOOK
		g_fLog << "Install shell hook!" << endl;
#endif
	}
	return hr;
}

BOOL WINAPI UnInstallShellHook()
{
	BOOL hr = FALSE;
	if (g_hHook) 
	{
		hr = UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
#ifdef USE_SHELLHOOK_MAP
#else
		memset(g_hookEntries, 0, sizeof(g_hookEntries));
		g_Index = g_Size = 0;
#endif
	}
	return hr;
}

BOOL WINAPI UnInstallShellHookEx(HWND hWnd)
{
	if (g_hDllInstance == NULL) return FALSE;
	BOOL hr = FALSE;
	if (hWnd)
	{
#ifdef USE_SHELLHOOK_MAP
		HANDLE hMutex = CreateMutex(NULL, TRUE, SHELLHOOK_MUTEX);
		SHELLHOOK_MAP::iterator it = g_hookMap.find(UINT(hWnd));
		if (it == g_hookMap.end())
		{
			return FALSE;
		}
		else
		{
			g_hookMap.erase(it);
#ifdef LOG_SHELLHOOK
			g_fLog << "Uninstall one hook entry!" << endl;
#endif	
		}
		if(g_hookMap.empty()) hr = UnInstallShellHook();
		else hr = TRUE;
		ReleaseMutex(hMutex);
#else
		hr = RemoveShellHookEntry(hWnd);
#ifdef LOG_SHELLHOOK
		// if (hr) g_fLog << "Uninstall one hook entry!" << endl;
#endif	
		if (g_Size == 0) hr = UnInstallShellHook();
		else hr = TRUE;
#endif
	}
	return hr;
}

LRESULT CALLBACK ShellHookProcEx(int nCode, WPARAM wParam, LPARAM lParam)
{
#ifdef LOG_SHELLHOOK
#ifdef USE_SHELLHOOK_MAP
	g_fLog << "Total " << g_hookMap.size() << " shell hook entries!" << endl;
#else
	g_fLog << "Total " << g_Size << " shell hook entries!" << endl;
#endif
#endif
#ifdef USE_SHELLHOOK_MAP
	for(SHELLHOOK_MAP::iterator it = g_hookMap.begin(); it != g_hookMap.end(); ++it)
	{
		SHELLHOOK_ENTRY entry = (*it).second;
		PostMessage(entry.hWnd, entry.uMsg, nCode, (LPARAM)wParam);
#ifdef LOG_SHELLHOOK
		g_fLog << "Post message to " << entry.hWnd << endl;
#endif
	}	
#else
	for(int i = 0; i < g_Size; i++)
	{
		PostMessage(g_hookEntries[i].hWnd, g_hookEntries[i].uMsg, nCode, (LPARAM)wParam);
#ifdef LOG_SHELLHOOK
		g_fLog << "Post message to " << g_hookEntries[i].hWnd << endl;
#endif
	}
#endif
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}