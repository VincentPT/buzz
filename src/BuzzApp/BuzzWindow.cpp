#include "BuzzWindow.h"
#include "utils/ClipboardViewer.h"

using namespace ci;
using namespace ci::app;

BuzzWindow::BuzzWindow(const std::string& title, int width, int height)
{
	ci::app::Window::Format format;	
	_nativeWindow = App::get()->createWindow(format);
	_nativeWindow->setUserData(this);

	setTitle(title);
	setSize(width, height);

	setupWindow();

}

BuzzWindow::BuzzWindow(ci::app::WindowRef nativeWindow) : _nativeWindow(nativeWindow) {
	_nativeWindow->setUserData(this);

	ClipboardViewer* clipboardViewer = ClipboardViewer::getInstance();
	// start monitor the clipboars
	clipboardViewer->startMonitor((HWND)_nativeWindow->getNative());

	setupWindow();
}

BuzzWindow::~BuzzWindow()
{
}

void BuzzWindow::setupWindow() {
	using namespace std::placeholders;

	// window settings gui
	_windowSettingsDlgRef = std::make_shared<WindowSettingsDlg>(_nativeWindow);
	_windowSettingsDlgRef->hide();

	// object settings gui
	_objectSettingsDlgRef = std::make_shared<ObjectSettingsDlg>(_nativeWindow);
	_objectSettingsDlgRef->hide();

	// add object gui
	_objectInputerDlgRef = std::make_shared<ObjectInputerDlg>(_nativeWindow);
	_objectInputerDlgRef->hide();
	_objectInputerDlgRef->setObjectAddress((void*)0x12345678);

	_bckColor = &_windowSettingsDlgRef->getBckColor();
	
	// setup user's event
	_nativeWindow->getSignalKeyDown().connect(std::bind(&BuzzWindow::onKeyPress, this, _1));

	_nativeWindow->getSignalClose().connect(std::bind(&BuzzWindow::onClose, this));
}

BuzzWindow& BuzzWindow::setTitle(const std::string& title) {
	_nativeWindow->setTitle(title);
	return *this;
}

BuzzWindow& BuzzWindow::setSize(int width, int height) {
	_nativeWindow->setSize(width, height);
	return *this;
}

void BuzzWindow::draw() {
	gl::clear(*_bckColor);
	_windowSettingsDlgRef->display();
	_objectSettingsDlgRef->display();
	_objectInputerDlgRef->display();
}

void BuzzWindow::showInputerWithAddress(void* address) {	
	_objectInputerDlgRef->setObjectAddress(address);
	_objectInputerDlgRef->show();
}

void BuzzWindow::onAddObjectButtonPress() {

}

void BuzzWindow::onKeyPress(KeyEvent& e) {
	if (!_activeDialog || _activeDialog->isVisible() == false) {
		if (e.getCode() == KeyEvent::KEY_w) {
			_windowSettingsDlgRef->show();
			_activeDialog = _windowSettingsDlgRef;
			e.setHandled();
		}
		else if (e.getCode() == KeyEvent::KEY_e) {
			_objectSettingsDlgRef->show();
			_activeDialog = _objectSettingsDlgRef;
			e.setHandled();
		}
		else if (e.getCode() == KeyEvent::KEY_r) {
			_objectInputerDlgRef->show();
			_activeDialog = _objectInputerDlgRef;
			e.setHandled();
		}
		
	}
}

void BuzzWindow::onClose() {
	auto clipboardViewer = ClipboardViewer::getInstance();

	// check if current viewer is going to closed
	if (clipboardViewer->getCurrentViewer() != (HWND)_nativeWindow->getNative()) {
		return;
	}
	clipboardViewer->stopMonitor();

	// start monitor clipboard on another window
	auto app = App::get();
	auto windowCount = app->getNumWindows();
	if (windowCount < 2) {
		// no window left
		return;
	}

	// find another window available
	for (int i = (int)(windowCount - 1); i >= 0; i--) {
		auto nativeWindow = app->getWindowIndex(i);
		if (nativeWindow.get() != _nativeWindow.get()) {
			clipboardViewer->startMonitor((HWND)nativeWindow->getNative());
			break;
		}
	}
}
