#include "ObjectSettingsDlg.h"
#include "BuzzCustomGui.h"

using namespace ci;
using namespace ci::app;

ObjectSettingsDlg::ObjectSettingsDlg(ci::app::WindowRef window) :
	BuzzDialog("Object settings", 300, 200, window) {

	pretzel::PretzelGuiRef nativeDlg = getNative();

	nativeDlg->addColorPicker("object background color", &_objBckColor);
	nativeDlg->addColorPicker("object line color", &_objLineColor);
	nativeDlg->addSlider("line with", &_lineWidth, 1, 10);
}
ObjectSettingsDlg::~ObjectSettingsDlg() {}

const ci::ColorA& ObjectSettingsDlg::getObjectBckColor() const {
	return _objBckColor;
}

const ci::ColorA& ObjectSettingsDlg::getObjectLineColor() const {
	return _objLineColor;
}

// set object's background color
void ObjectSettingsDlg::setObjectBckColor(const ci::ColorA& color) {
	_objBckColor = color;
}

// set object's line color
void ObjectSettingsDlg::setObjectLineColor(const ci::ColorA& color) {
	_objLineColor = color;
}

int ObjectSettingsDlg::getLineWidth() const {
	return _lineWidth;
}