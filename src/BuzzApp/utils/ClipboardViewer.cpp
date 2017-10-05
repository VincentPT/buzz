#include "ClipboardViewer.h"
#include <iostream>
using namespace std;

ClipboardViewer* ClipboardViewer::getInstance() {
	static ClipboardViewer clipboarViewer;
	return &clipboarViewer;
}

ClipboardViewer::ClipboardViewer()
{
	// Add the window to the clipboard viewer chain. 
	//_hwndNext = SetClipboardViewer(hwnd);
}

ClipboardViewer::~ClipboardViewer()
{
}


HWND ClipboardViewer::getCurrentViewer() const {
	return _hwnd;
}

LRESULT CALLBACK ClipboardViewer::wndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	ClipboardViewer* clbViewer = ClipboardViewer::getInstance();
	switch (uMsg) {
	case WM_CHANGECBCHAIN:
		// If the next window is closing, repair the chain.
		if ((HWND)wParam == clbViewer->_hwndNext) {
			clbViewer->_hwndNext = (HWND)lParam;			
		}
		// Otherwise, pass the message to the next link. 
		else if (clbViewer->_hwndNext != NULL) {
			SendMessage(clbViewer->_hwndNext, uMsg, wParam, lParam);
		}
		break;
	case WM_DRAWCLIPBOARD:
		// clipboard contents changed. 
		// Update the window by using Auto clipboard format.
		clbViewer->notifyClipboardChangeToHandlers();
		// Pass the message to the next window in clipboard 
		// viewer chain. 
		SendMessage(clbViewer->_hwndNext, uMsg, wParam, lParam);
		break;
	case WM_DESTROY:
		clbViewer->stopMonitor();
		return CallWindowProc(clbViewer->_wndprocPrev, wnd, uMsg, wParam, lParam);
	default:
		return CallWindowProc(clbViewer->_wndprocPrev, wnd, uMsg, wParam, lParam);
	}
	return (LRESULT)NULL;
}

bool ClipboardViewer::startMonitor() {
	// get current active window to be a clipboard viewer
	_hwnd = GetActiveWindow();
	return startMonitor(_hwnd);
}

bool ClipboardViewer::startMonitor(HWND hWnd) {
	// get current active window to be a clipboard viewer
	_hwnd = hWnd;
	if (_hwnd == nullptr) {
		return false;
	}

	// install a sub window proc to monitor window's messages
	_wndprocPrev = (WNDPROC)SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)ClipboardViewer::wndProc);
	if (_wndprocPrev == nullptr) {
		return false;
	}

	// Add the window to the clipboard viewer chain. 
	_hwndNext = SetClipboardViewer(_hwnd);

	return true;
}

void ClipboardViewer::stopMonitor() {
	if (_wndprocPrev) {
		SetWindowLongPtr(_hwnd, GWLP_WNDPROC, (LONG_PTR)_wndprocPrev);
	}

	ChangeClipboardChain(_hwnd, _hwndNext);

	_hwnd = nullptr;
}

void ClipboardViewer::connectANSI(std::function<void(const std::string&)>&& ansiTextdHandler) {
	_ansiTextHandler = ansiTextdHandler;
	auto it = std::find(_registeredFormats.begin(), _registeredFormats.end(), CF_TEXT);
	if (it == _registeredFormats.end()) {
		_registeredFormats.push_back(CF_TEXT);
	}
}

// register monitor unicode text format in clipboard
void ClipboardViewer::connectUnicode(std::function<void(const std::wstring&)>&& unicodeTextHandler) {
	_unicodeTextHandler = unicodeTextHandler;
	auto it = std::find(_registeredFormats.begin(), _registeredFormats.end(), CF_UNICODETEXT);
	if (it == _registeredFormats.end()) {
		_registeredFormats.push_back(CF_UNICODETEXT);
	}
}

// register monitor bitmap format in clipboard
void ClipboardViewer::connectBitmap(std::function<void(HBITMAP)>&& bitmapHandler) {
	_bitmapHandler = bitmapHandler;
	auto it = std::find(_registeredFormats.begin(), _registeredFormats.end(), CF_BITMAP);
	if (it == _registeredFormats.end()) {
		_registeredFormats.push_back(CF_BITMAP);
	}
}

void ClipboardViewer::notifyClipboardChangeToHandlers() {
	auto format = GetPriorityClipboardFormat(_registeredFormats.data(), (int)_registeredFormats.size());
	HGLOBAL hglb;
	LPSTR lpstr;
	LPWSTR lpwstr;
	HBITMAP hbm;

	switch (format)
	{
	case CF_TEXT:
		cout << "detect CF_TEXT changed" << std::endl;
		if (OpenClipboard(_hwnd))
		{
			hglb = GetClipboardData(format);
			lpstr = (LPSTR)GlobalLock(hglb);

			_ansiTextHandler(lpstr);

			GlobalUnlock(hglb);
			CloseClipboard();
		}
		else {
			cout << "!!!Error: Cannot open the clipboard, GLE=" << GetLastError() << std::endl;
		}
		break;
	case CF_UNICODETEXT:
		cout << "detect CF_UNICODETEXT changed" << std::endl;
		if (OpenClipboard(_hwnd))
		{
			hglb = GetClipboardData(format);
			lpwstr = (LPWSTR)GlobalLock(hglb);

			_unicodeTextHandler(lpwstr);

			GlobalUnlock(hglb);
			CloseClipboard();
		}
		else {
			cout << "!!!Error: Cannot open the clipboard, GLE=" << GetLastError() << std::endl;
		}		
		break;
	case CF_BITMAP:
		cout << "detect CF_BITMAP changed" << std::endl;
		if (OpenClipboard(_hwnd))
		{
			hbm = (HBITMAP)
				GetClipboardData(format);

			_bitmapHandler(hbm);

			CloseClipboard();
		}
		else {
			cout << "!!!Error: Cannot open the clipboard, GLE=" << GetLastError() << std::endl;
		}
		break;
	default:
		cout << "Unsupport clipboard format" << std::endl;
		break;
	}
}

void ClipboardViewer::clearHandlers() {
	_registeredFormats.clear();
}