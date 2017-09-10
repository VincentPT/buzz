#pragma once
#include "BuzzDialog.h"

class WindowSettingsDlg : public BuzzDialog {
	// window's background color
	ci::ColorA					_bckColor;
private:

public:
	WindowSettingsDlg(ci::app::WindowRef window);
	~WindowSettingsDlg();

	// return window's background color
	ci::ColorA getBckColor() const;
	ci::ColorA& getBckColor();

	// set window's background color
	void setBckColor(const ci::ColorA&);
};