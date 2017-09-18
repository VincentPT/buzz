#include "BuzzCustomGui.h"
using namespace ci;
using namespace ci::app;

BuzzCustomGui::BuzzCustomGui(const std::string& title, int width, int height, ci::app::WindowRef window) : pretzel::PretzelGui(title, width, height, window) {
}
void BuzzCustomGui::keyDown(const char &key, const int &keyCode) {
	if (keyCode == KeyEvent::KEY_ESCAPE) {
		this->setVisible(false);
		if (_closeEventHandler) {
			_closeEventHandler();
		}
		return;
	}
	pretzel::PretzelGui::keyDown(key, keyCode);
}

void BuzzCustomGui::setCloseEventHandler(CloseEventHandler&& eventHandler) {
	_closeEventHandler = eventHandler;
}