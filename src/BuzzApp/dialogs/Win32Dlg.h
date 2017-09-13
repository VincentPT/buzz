#pragma once
#include <Windows.h>

class Win32Dlg {
	static INT_PTR CALLBACK DialogProc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);

protected:
	HWND _hwndParent;
	HWND _hDlg;
	UINT _dialogId;
protected:
	virtual INT_PTR processMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void onCancle();
	virtual void onInit();
	virtual void onDestroy();
public:
	Win32Dlg(HWND hwndParent, UINT idd);
	virtual ~Win32Dlg();

	virtual void show();
	virtual void hide();
};