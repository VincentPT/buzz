#pragma once
#include <string>
#include <list>
#include "cinder/app/Renderer.h"
#include "cinder/gl/gl.h"
#include "dialogs/ObjectInputerDlg.h"
#include "dialogs/ObjectSettingsDlg.h"
#include "dialogs/WindowSettingsDlg.h"
#include "utils/BuzzSpyClient.h"
#include "drawobjs/BuzzDrawObj.h"


class BuzzWindow
{
	std::shared_ptr<BuzzSpyClient>			_spyClient;
	bool									_needUpdate = true;
	bool									_pendingUpdate = false;
	bool									_pendingResize = false;
	double									_updateFBOIV = 0.25f; //time in seconds to update FBO after an user's continous event
	double									_lastEventOccur = 0; // time in seconds that last continous event occur

	ci::app::WindowRef						_nativeWindow;	
	ci::ColorA*								_bckColor;
	ci::gl::Texture2dRef					_tex;
	ci::gl::FboRef							_frameBuffer;
	glm::mat4								_transform;
	glm::mat4								_texTransform;

	std::shared_ptr<WindowSettingsDlg>		_windowSettingsDlgRef;
	std::shared_ptr<ObjectSettingsDlg>		_objectSettingsDlgRef;
	std::shared_ptr<ObjectInputerDlg>		_objectInputerDlgRef;
	std::shared_ptr<BuzzDialog>				_activeDialog;
	std::list<BuzzDrawObjRef>				_drawingObjects;
private:
	void onKeyPress(ci::app::KeyEvent& e);
	void onClose();
	void onResize();
	void onMouseWheel(ci::app::MouseEvent& me);
	void onStartMonitorProcess(BuzzDialog* sender);
	void onAddObjectClick(BuzzDialog* sender);	
protected:
	void setupWindow();
	void needUpdate(bool flag = true);
	void pendingUpdate();
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
	void readCVMatObject(void* desireReadObjectAddress);
	void readCVPointObject(void* desireReadObjectAddress);
	void readCVPoint2fObject(void* desireReadObjectAddress);
	void readCVRectObject(void* desireReadObjectAddress);
	void readCVContourObject(void* desireReadObjectAddress);
	void readCVContoursObject(void* desireReadObjectAddress);
};

typedef std::shared_ptr<BuzzWindow> BuzzWindowRef;
