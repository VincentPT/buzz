#pragma once
#include <string>
#include "cinder/app/Renderer.h"
#include "cinder/gl/gl.h"
#include "dialogs/ObjectInputerDlg.h"
#include "dialogs/ObjectSettingsDlg.h"
#include "dialogs/WindowSettingsDlg.h"
#include "utils/BuzzSpyClient.h"

class BuzzWindow
{
	ci::app::WindowRef						_nativeWindow;	
	ci::ColorA*								_bckColor;
	ci::gl::Texture2dRef					_tex;

	std::shared_ptr<WindowSettingsDlg>		_windowSettingsDlgRef;
	std::shared_ptr<ObjectSettingsDlg>		_objectSettingsDlgRef;
	std::shared_ptr<ObjectInputerDlg>		_objectInputerDlgRef;
	std::shared_ptr<BuzzDialog>				_activeDialog;

	std::shared_ptr<BuzzSpyClient>			_spyClient;
private:
	void onKeyPress(ci::app::KeyEvent& e);
	void onClose();
	void onStartMonitorProcess(BuzzDialog* sender);
	void onAddObjectClick(BuzzDialog* sender);
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

	void readObject(void* desireReadObjectAddress, int type);
	void readMatObject(void* desireReadObjectAddress);
};

typedef std::shared_ptr<BuzzWindow> BuzzWindowRef;
