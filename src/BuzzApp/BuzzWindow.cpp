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
}

BuzzWindow::BuzzWindow(ci::app::WindowRef nativeWindow) : _nativeWindow(nativeWindow) {
	_nativeWindow->setUserData(this);

	setupWindow();
}

BuzzWindow::~BuzzWindow()
{
}

void BuzzWindow::setupWindow() {
}

BuzzWindow& BuzzWindow::setTitle(const std::string& title) {
	_nativeWindow->setTitle(title);
	return *this;
}

BuzzWindow& BuzzWindow::setSize(int width, int height) {
	_nativeWindow->setSize(width, height);
	return *this;
}