#include "Win32Dlg.h"

INT_PTR CALLBACK Win32Dlg::DialogProc(_In_ HWND hwndDlg, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	if (WM_INITDIALOG == uMsg) {
		Win32Dlg* theDialog = (Win32Dlg*)lParam;
		theDialog->_hDlg = hwndDlg;

		SetWindowLongPtr(hwndDlg, GWLP_USERDATA, lParam);

		theDialog->onInit();
		return TRUE;
	}
	
	Win32Dlg* theDialog = (Win32Dlg*) GetWindowLongPtr(hwndDlg, GWLP_USERDATA);
	if (theDialog) {
		return theDialog->processMessage(hwndDlg, uMsg, wParam, lParam);
	}
	return FALSE;
}

INT_PTR Win32Dlg::processMessage(HWND hwndDlg, UINT uMsg, WPARAM wParam,  LPARAM lParam) {
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			return TRUE;
		case IDCANCEL:
			onCancle();
			return TRUE;
		}
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE) {
			onCancle();
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Win32Dlg::onCancle() {
	onDestroy();

	DestroyWindow(_hDlg);
	_hDlg = NULL;
}

void Win32Dlg::onInit() {
}

void Win32Dlg::onDestroy() {
}

Win32Dlg::Win32Dlg(HWND hwndParent, UINT idd) : _hwndParent(nullptr), _dialogId(idd) {
}

Win32Dlg::~Win32Dlg() {
	if (_hDlg != NULL) {
		onCancle();
		DestroyWindow(_hDlg);
	}
}

void Win32Dlg::show() {
	HMODULE hModule = GetModuleHandleA(NULL);
	//DialogBoxParamA(hModule, MAKEINTRESOURCEA(_dialogId), _hwndParent, &Win32Dlg::DialogProc, (LPARAM)this);
	_hDlg = CreateDialogParamA(hModule, MAKEINTRESOURCEA(_dialogId), NULL, &Win32Dlg::DialogProc, (LPARAM)this);
	SetWindowLongPtr(_hDlg, GWLP_USERDATA, (LPARAM)this);
	::ShowWindow(_hDlg, SW_SHOW);
}

void Win32Dlg::hide() {

}