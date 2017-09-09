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
	_bckColor = Color::white();
	// window settings gui
	_windowSettings = pretzel::PretzelGui::create("Window settings", 300, 500, _nativeWindow);
	_windowSettings->setVisible(false);

	// object settings gui
	_objectSettings = pretzel::PretzelGui::create("Object settings", 300, 500, _nativeWindow);
	_objectSettings->setVisible(false);

	// add object gui
	_objecInputer = pretzel::PretzelGui::create("Add object", 300, 500, _nativeWindow);
	_objecInputer->setVisible(false);

	// add components to window settings gui
	_windowSettings->addColorPicker("Background Color", &_bckColor);
	_windowSettings->addSaveLoad();
	_windowSettings->loadSettings();

	// add components to object settings gui
	_objectSettings->addColorPicker("object background color", &_objBckColor);
	_objectSettings->addColorPicker("object line color", &_objLineColor);

	// add components to adding object gui
	_objectTypes = { "OpenCV Mat", "OpenCV Rect", "OpenCV Contour" };
	_objecInputer->addTextField("Address", &_objectAddress, true);
	_objecInputer->addEnum("Object type", &_objectTypes, &_objectTypeIdx);
	_objecInputer->addButton("Add", &BuzzWindow::onAddObjectButtonPress, this);
	_objecInputer->addButton("Close", &BuzzWindow::onCloseButtonPress, this);

	// setup user's event
	_nativeWindow->getSignalKeyDown().connect([this](KeyEvent& e) {
		if (e.getCode() == KeyEvent::KEY_b) {
			_windowSettings->setVisible(true);
			//_objectSettings->setVisible(true);
			//_objecInputer->setVisible(true);
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
	gl::clear(_bckColor);
	_windowSettings->draw();
	_objectSettings->draw();
	_objecInputer->draw();
}

void BuzzWindow::onAddObjectButtonPress() {

}

void BuzzWindow::onCloseButtonPress() {
	_objecInputer->setVisible(false);
}