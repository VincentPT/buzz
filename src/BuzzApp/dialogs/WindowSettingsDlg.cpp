#include "WindowSettingsDlg.h"
#include "BuzzCustomGui.h"

using namespace ci;
using namespace ci::app;

WindowSettingsDlg::WindowSettingsDlg(ci::app::WindowRef window) :
	BuzzDialog("Window settings", 300, 200, window) {
	pretzel::PretzelGuiRef nativeDlg = getNative();

	// add components to window settings gui
	nativeDlg->addColorPicker("Background Color", &_bckColor);
	nativeDlg->addTextField("process name", &_processName, true);
	nativeDlg->addButton("monitor", &WindowSettingsDlg::onMonitorButtonPress, this);

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
WindowSettingsDlg& WindowSettingsDlg::setBckColor(const ci::ColorA& color) {
	_bckColor = color;
	return *this;
}

std::string WindowSettingsDlg::getProcessName() const {
	return _processName;
}

WindowSettingsDlg& WindowSettingsDlg::setProcessName(const std::string& processName) {
	_processName = processName;
	return *this;
}

void WindowSettingsDlg::onMonitorButtonPress() {

}