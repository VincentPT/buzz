#pragma once
#include <Windows.h>
#include <vector>
#include <functional>
#include <string>

class ClipboardViewer
{
	HWND _hwnd;
	HWND _hwndNext;
	WNDPROC _wndprocPrev;
	std::vector<UINT> _registeredFormats;
	std::function<void(const std::string&)> _ansiTextHandler;
	std::function<void(const std::wstring&)> _unicodeTextHandler;
	std::function<void(HBITMAP)> _bitmapHandler;
private:
	ClipboardViewer();
	static LRESULT CALLBACK wndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void notifyClipboardChangeToHandlers();
public:	
	static ClipboardViewer* getInstance();
	~ClipboardViewer();

	HWND getCurrentViewer() const;

	// using current active window as clipboard viewer
	bool startMonitor();

	// using hWnd as clipboard viewer
	bool startMonitor(HWND hWnd);
	void stopMonitor();

	// register monitor ansi text format in clipboard
	void connect(std::function<void(const std::string&)>&& ansiTextdHandler);
	// register monitor unicode text format in clipboard
	void connect(std::function<void(const std::wstring&)>&& unicodeTextHandler);
	// register monitor bitmap format in clipboard
	void connect(std::function<void(HBITMAP)>&& bitmapHandler);
};

