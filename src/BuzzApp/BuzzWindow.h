#pragma once
#include <string>
#include "cinder/app/Renderer.h"
#include "cinder/gl/gl.h"
#include "dialogs/ObjectInputerDlg.h"
#include "dialogs/ObjectSettingsDlg.h"
#include "dialogs/WindowSettingsDlg.h"

class BuzzWindow
{
	ci::app::WindowRef						_nativeWindow;	
	ci::ColorA*								_bckColor;

	std::shared_ptr<WindowSettingsDlg>		_windowSettingsDlgRef;
	std::shared_ptr<ObjectSettingsDlg>		_objectSettingsDlgRef;
	std::shared_ptr<ObjectInputerDlg>		_objectInputerDlgRef;
	std::shared_ptr<BuzzDialog>				_activeDialog;
	
private:
	void onAddObjectButtonPress();
	void onKeyPress(ci::app::KeyEvent& e);
	void onClose();
protected:
	void setupWindow();
public:
	BuzzWindow(const std::string& title, int width, int height);
	BuzzWindow(ci::app::WindowRef nativeWindow);
	~BuzzWindow();

	BuzzWindow& setTitle(const std::string& title);
	BuzzWindow& setSize(int width, int height);

	void showInputerWithAddress(void* address);

	/// this function is should only should only be called by draw function of a cinder app
	void draw();
};

typedef std::shared_ptr<BuzzWindow> BuzzWindowRef;
