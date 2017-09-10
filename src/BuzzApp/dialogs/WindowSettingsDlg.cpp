#include "WindowSettingsDlg.h"
#include "BuzzCustomGui.h"

using namespace ci;
using namespace ci::app;

WindowSettingsDlg::WindowSettingsDlg(ci::app::WindowRef window) :
	BuzzDialog("Window settings", 300, 500, window) {
	pretzel::PretzelGuiRef nativeDlg = getNative();

	// add components to window settings gui
	nativeDlg->addColorPicker("Background Color", &_bckColor);
	nativeDlg->addSaveLoad();
	nativeDlg->loadSettings();
}
WindowSettingsDlg::~WindowSettingsDlg() {}

ci::ColorA WindowSettingsDlg::getBckColor() const {
	return _bckColor;
}

ci::ColorA& WindowSettingsDlg::getBckColor() {
	return _bckColor;
}

// set object's background color
void WindowSettingsDlg::setBckColor(const ci::ColorA& color) {
	_bckColor = color;
}