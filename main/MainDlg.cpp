// MainDlg.cpp: implementation of the CMainDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"

#pragma comment (lib, "ShellHook.lib")
extern BOOL WINAPI InstallShellHook(HWND hWnd, UINT uMsg);
extern BOOL WINAPI UnInstallShellHook();
extern BOOL WINAPI InstallShellHookEx(HWND hWnd, UINT uMsg);
extern BOOL WINAPI UnInstallShellHookEx(HWND hWnd);

CMainDlg* g_pMainWin = 0;

LRESULT CMainDlg::OnInitDialog(
	UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CenterWindow();
	HRESULT hr = S_OK;
	bHandled = FALSE;
	return TRUE;
}

LRESULT CMainDlg::OnDestroy(
	UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	UnInstallShellHookEx(m_hWnd);
	UnInstallShellHook();
	PostQuitMessage(0);
	return 0;
}

LRESULT CMainDlg::OnShell(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CString strDebug, strTitle;
	CWnd* pwnd;
	if(::IsWindow(HWND(lParam)))
	{
		pwnd = CWnd::FromHandle(HWND(lParam));
		pwnd->GetWindowText(strTitle);
	}
	switch (wParam)
	{
	case HSHELL_WINDOWCREATED:
		strDebug.Format("Window[%08X] - %s created!\n", lParam, strTitle);
		break;
	case HSHELL_WINDOWDESTROYED:
		strDebug.Format("Window[%08X] - %s destroyed!\n", lParam, strTitle);
		break;
	case HSHELL_WINDOWACTIVATED:
		strDebug.Format("Window[%08X] - %s activated!\n", lParam, strTitle);
		break;
	default:
		strDebug.Format("%08X, %08X\n", wParam, lParam);
		break;
	}
	Log(strDebug);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT CMainDlg::OnHook(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if(InstallShellHook(m_hWnd, WM_SHELL))
	{
		Log("Shell hook installed successfully!\n");
	}
	else
	{
		Log("Can NOT install shell hook, maybe this window has already installed one!\n");
	}
	return 0;
}

LRESULT CMainDlg::OnHookEx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	if (InstallShellHookEx(m_hWnd, WM_SHELL))
	{
		Log("Shell hookex installed successfully!\n");
	}
	else
	{
		Log("Can NOT install shell hookex, maybe this window has already installed one!\n");
	}
	return 0;
}

LRESULT CMainDlg::OnUnhook(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UnInstallShellHook();
	return 0;
}

LRESULT CMainDlg::OnUnhookEx(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UnInstallShellHookEx(m_hWnd);
	return 0;
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

void test1()
{
	HWND hWnd = HWND(0x00010412);
	HWND hAncestorWnd = GetAncestor(hWnd, GA_ROOTOWNER);
	CString strDebug;
	strDebug.Format("hWnd = %08X, hAncestorWnd = %08X\n", hWnd, hAncestorWnd);
	g_pMainWin->Log(strDebug);
}

LRESULT CMainDlg::OnTest(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CWindow wndEdit = GetDlgItem(IDC_LOG);
	int len = wndEdit.GetWindowTextLength();
	wndEdit.SendMessage(EM_SETSEL, 0, len);
	wndEdit.SendMessage(EM_REPLACESEL, FALSE, NULL);
	return S_OK;
}

void CMainDlg::Log(LPCTSTR psz)
{
	CWindow wndEdit = GetDlgItem(IDC_LOG);
	int len = wndEdit.GetWindowTextLength();
	wndEdit.SendMessage(EM_SETSEL, len, len);
	wndEdit.SendMessage(EM_REPLACESEL, FALSE, reinterpret_cast<LPARAM>(psz));
}
