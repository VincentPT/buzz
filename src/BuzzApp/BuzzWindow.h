#pragma once
#include <string>
#include "cinder/app/Renderer.h"
#include "cinder/gl/gl.h"
#include "pretzel/PretzelGui.h"

class BuzzWindow
{
	ci::app::WindowRef			_nativeWindow;
	pretzel::PretzelGuiRef		_windowSettings;
	pretzel::PretzelGuiRef		_objectSettings;
	pretzel::PretzelGuiRef		_objecInputer;

	ci::ColorA					_bckColor;

	ci::ColorA					_objBckColor;
	ci::ColorA					_objLineColor;

	std::string					_objectAddress;
	std::vector<std::string>    _objectTypes;
	int                         _objectTypeIdx = 0;
private:
	void onAddObjectButtonPress();
	void onCloseButtonPress();
protected:
	void setupWindow();
public:
	BuzzWindow(const std::string& title, int width, int height);
	BuzzWindow(ci::app::WindowRef nativeWindow);
	~BuzzWindow();

	BuzzWindow& setTitle(const std::string& title);
	BuzzWindow& setSize(int width, int height);

	/// this function is should only should only be called by draw function of a cinder app
	void draw();
};

typedef std::shared_ptr<BuzzWindow> BuzzWindowRef;
