#include "BuzzWindow.h"
#include "BuzzApp.h"

using namespace ci;
using namespace ci::app;

BuzzWindow::BuzzWindow(const std::string& title, int width, int height)
{
	ci::app::Window::Format format;	
	_nativeWindow = BuzzApp::getInstance()->createWindow(format);
	_nativeWindow->setUserData(this);

	setTitle(title);
	setSize(width, height);

	setupWindow();

}

BuzzWindow::BuzzWindow(ci::app::WindowRef nativeWindow) : _nativeWindow(nativeWindow) {
	_nativeWindow->setUserData(this);

	setupWindow();
}

BuzzWindow::~BuzzWindow()
{
}

void BuzzWindow::setupWindow() {
	// window settings gui
	_windowSettingsDlgRef = std::make_shared<WindowSettingsDlg>(_nativeWindow);
	_windowSettingsDlgRef->hide();

	// object settings gui
	_objectSettingsDlgRef = std::make_shared<ObjectSettingsDlg>(_nativeWindow);
	_objectSettingsDlgRef->hide();

	// add object gui
	_objectInputerDlgRef = std::make_shared<ObjectInputerDlg>(_nativeWindow);
	_objectInputerDlgRef->hide();

	_bckColor = &_windowSettingsDlgRef->getBckColor();

	// setup user's event
	_nativeWindow->getSignalKeyDown().connect([this](KeyEvent& e) {
		if (e.getCode() == KeyEvent::KEY_b) {
			_windowSettingsDlgRef->show();
		}
		else if (e.getCode() == KeyEvent::KEY_o) {
			_objectSettingsDlgRef->show();
		}
		else if (e.getCode() == KeyEvent::KEY_i) {
			_objectInputerDlgRef->show();
		}
	});
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

void BuzzWindow::onAddObjectButtonPress() {

}