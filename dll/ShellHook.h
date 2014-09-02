#if !defined(SHELLHOOK__INCLUDED_)
#define SHELLHOOK__INCLUDED_

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SHELLHOOK_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SHELLHOOK_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef SHELLHOOK_EXPORTS
#define SHELLHOOK_API __declspec(dllexport)
#else
#define SHELLHOOK_API __declspec(dllimport)
#endif

extern BOOL WINAPI InstallShellHook(HWND hWnd, UINT uMsg);
extern BOOL WINAPI UnInstallShellHook();
extern BOOL WINAPI InstallShellHookEx(HWND hWnd, UINT uMsg);
extern BOOL WINAPI UnInstallShellHookEx(HWND hWnd);

#endif // !defined(SHELLHOOK__INCLUDED_)