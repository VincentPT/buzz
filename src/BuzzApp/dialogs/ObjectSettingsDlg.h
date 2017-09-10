#pragma once
#include "BuzzDialog.h"
#include <string>
#include <vector>

class ObjectSettingsDlg : public BuzzDialog {

	// object's background color
	ci::ColorA					_objBckColor;
	// object's line color
	ci::ColorA					_objLineColor;
private:

public:
	ObjectSettingsDlg(ci::app::WindowRef window);
	~ObjectSettingsDlg();

	// return object's background color
	ci::ColorA getObjectBckColor() const;

	// return object's line color
	ci::ColorA getObjectLineColor() const;

	// set object's background color
	void setObjectBckColor(const ci::ColorA&);

	// set object's line color
	void setObjectLineColor(const ci::ColorA&);
};