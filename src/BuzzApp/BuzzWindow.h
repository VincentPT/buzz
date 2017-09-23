#pragma once
#include <string>
#include <list>
#include "cinder/app/Renderer.h"
#include "cinder/gl/gl.h"
#include "dialogs/ObjectInputerDlg.h"
#include "dialogs/ObjectSettingsDlg.h"
#include "dialogs/WindowSettingsDlg.h"
#include "utils/BuzzSpyClient.h"
#include "drawobjs/BuzzContainer.h"
#include "dialogs/ObjectHierarchyDlg.h"
#include "dialogs/ObjectHierarchyDlg2.h"


class BuzzWindow
{
	std::shared_ptr<BuzzSpyClient>			_spyClient;
	bool									_needUpdate = true;
	bool									_afterUpdate = false;
	bool									_pendingUpdate = false;
	bool									_pendingResize = false;
	double									_updateFBOIV = 0.25f; //time in seconds to update FBO after an user's continous event
	double									_lastEventOccur = 0; // time in seconds that last continous event occur

	ci::app::WindowRef						_nativeWindow;	
	ci::ColorA*								_bckColor;
	ci::ColorA								_objectLine;
	ci::gl::Texture2dRef					_tex;
	ci::gl::FboRef							_frameBuffer;
	glm::mat4								_transform;
	glm::mat4								_texTransform;

	std::shared_ptr<WindowSettingsDlg>		_windowSettingsDlgRef;
	std::shared_ptr<ObjectSettingsDlg>		_objectSettingsDlgRef;
	std::shared_ptr<ObjectInputerDlg>		_objectInputerDlgRef;
	std::shared_ptr<BuzzDialog>				_activeDialog;
	std::shared_ptr<BuzzContainer>			_rootObject;
	std::shared_ptr<ObjectHierarchyDlg>		_hierarchyDialog;
	std::shared_ptr<ObjectHierarchyDlg2>	_hierarchyDialog2;
private:
	void onKeyPress(ci::app::KeyEvent& e);
	void onClose();
	void onResize();
	void onMouseWheel(ci::app::MouseEvent& me);
	void onStartMonitorProcess(BuzzDialog* sender);
	void onAddObjectClick(BuzzDialog* sender);
	void onObjectCheckedChanged(BuzzDrawObj* obj, BOOL newState);
protected:
	void setupWindow();	
	void pendingUpdate();
public:
	BuzzWindow(const std::string& title, int width, int height);
	BuzzWindow(ci::app::WindowRef nativeWindow);
	~BuzzWindow();

	void needUpdate(bool flag = true);

	BuzzWindow& setTitle(const std::string& title);
	BuzzWindow& setSize(int width, int height);

	void showInputerWithAddress(void* address);

	/// this function is should only should only be called by draw function of a cinder app
	void draw();

	void addNewObject(BuzzDrawObj* obj);

	BuzzDrawObj* readObject(void* desireReadObjectAddress, int type);
	BuzzDrawObj* readCVMatObject(void* desireReadObjectAddress);
	BuzzDrawObj* readCVPointObject(void* desireReadObjectAddress);
	BuzzDrawObj* readCVPoint2fObject(void* desireReadObjectAddress);
	BuzzDrawObj* readCVRectObject(void* desireReadObjectAddress);
	BuzzDrawObj* readCVContourObject(void* desireReadObjectAddress);
	BuzzDrawObj* readCVContoursObject(void* desireReadObjectAddress);
	BuzzDrawObj* readDummyTree(void* desireReadObjectAddress);
};

typedef std::shared_ptr<BuzzWindow> BuzzWindowRef;
