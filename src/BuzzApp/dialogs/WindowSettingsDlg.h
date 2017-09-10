#pragma once
#include "BuzzDialog.h"

class WindowSettingsDlg : public BuzzDialog {
	// window's background color
	ci::ColorA					_bckColor;
	std::string					_processName;
private:
	void onMonitorButtonPress();
public:
	WindowSettingsDlg(ci::app::WindowRef window);
	~WindowSettingsDlg();

	// return window's background color
	ci::ColorA getBckColor() const;
	ci::ColorA& getBckColor();

	// set window's background color
	WindowSettingsDlg& setBckColor(const ci::ColorA&);

	std::string getProcessName() const;
	WindowSettingsDlg& setProcessName(const std::string& processName);
};