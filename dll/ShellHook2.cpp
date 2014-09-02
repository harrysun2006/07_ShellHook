// ShellHook.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "ShellHook.h"

#define SHELLHOOK_ENTRY_MAX			128
#define SHELLHOOK_ENTRY_NOTFOUND	-1
#define SHELLHOOK_MUTEX				"SHELL_HOOK_MUTEX"

typedef struct SHELLHOOK_ENTRY {
//	HWND hAncestorWnd;
	HWND hWnd;
	UINT uMsg;
} SHELLHOOK_ENTRY;

typedef SHELLHOOK_ENTRY* LPSHELLHOOK_ENTRY;

// #pragma声明一个数据段, 在此范围内声明的变量在初始化后将被指派到该数据段;
// 如未初始化, 变量将被分配到缺省数据段, 而#pragma不起作用。
// map等C++对象在共享数据段中初始化使用有问题(大小不确定)
#pragma data_seg("mydata")
HINSTANCE g_hDllInstance = NULL;
int g_Index = 0, g_Size = 0;
HHOOK g_hHook = NULL;
SHELLHOOK_ENTRY g_hookEntries[SHELLHOOK_ENTRY_MAX] = {0};
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
			break;
	}
	return TRUE;
}

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

BOOL WINAPI InstallShellHook(HWND hWnd, UINT uMsg)  
{
	return InstallShellHookEx(hWnd, uMsg);
}

BOOL WINAPI InstallShellHookEx(HWND hWnd, UINT uMsg)  
{
	if (g_hDllInstance == NULL) return FALSE;
	if (g_Index < 0) return FALSE;
	int index = AddShellHookEntry(hWnd);
	if (index < 0 || index >= SHELLHOOK_ENTRY_MAX) return FALSE;
	if (g_hookEntries[index].hWnd) return FALSE;
	g_hookEntries[index].hWnd = hWnd;
	g_hookEntries[index].uMsg = uMsg;
	if (g_hHook == NULL)
	{
		HHOOK hhook = NULL;
		hhook = SetWindowsHookEx(WH_SHELL, ShellHookProcEx, g_hDllInstance, 0);
		InterlockedExchange((PLONG)&g_hHook, (LONG)hhook);
	}
	return TRUE;
}

BOOL WINAPI UnInstallShellHook()
{
	BOOL hr = FALSE;
	if (g_hHook) 
	{
		hr = UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
		memset(g_hookEntries, 0, sizeof(g_hookEntries));
		g_Index = g_Size = 0;
	}
	return hr;
}

BOOL WINAPI UnInstallShellHookEx(HWND hWnd)
{
	if (g_hDllInstance == NULL) return FALSE;
	BOOL hr = FALSE;
	if (hWnd)
	{
		hr = RemoveShellHookEntry(hWnd);
		if (g_Size == 0) hr = UnInstallShellHook();
		else hr = TRUE;
	}
	return hr;
}

LRESULT CALLBACK ShellHookProcEx(int nCode, WPARAM wParam, LPARAM lParam)
{
	for(int i = 0; i < g_Size; i++)
	{
		PostMessage(g_hookEntries[i].hWnd, g_hookEntries[i].uMsg, nCode, (LPARAM)wParam);
	}
	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}