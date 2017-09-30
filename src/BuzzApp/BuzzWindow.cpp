#include "BuzzWindow.h"
#include "utils/ClipboardViewer.h"
#include "pretzel/PretzelGui.h"
#include "drawobjs/BuzzContainer.h"
#include "drawobjs/BuzzHasLine.h"

using namespace ci;
using namespace ci::app;
using namespace std;

BuzzWindow::BuzzWindow(const std::string& title, int width, int height)
{
	ci::app::Window::Format format;
	_nativeWindow = App::get()->createWindow(format);
	_nativeWindow->setUserData(this);

	setTitle(title);
	setSize(width, height);

	setupWindow();

}

BuzzWindow::BuzzWindow(ci::app::WindowRef nativeWindow) : _nativeWindow(nativeWindow) {
	_nativeWindow->setUserData(this);

	ClipboardViewer* clipboardViewer = ClipboardViewer::getInstance();
	// start monitor the clipboars
	clipboardViewer->startMonitor((HWND)_nativeWindow->getNative());

	setupWindow();
}

BuzzWindow::~BuzzWindow()
{
	_spyClient->stopMonitorProcess();
}

void BuzzWindow::setupWindow() {
	using namespace std::placeholders;
	auto size = _nativeWindow->getSize();
	_frameBuffer = gl::Fbo::create(size.x, size.y, true, true, true);

	// window settings gui
	_windowSettingsDlgRef = std::make_shared<WindowSettingsDlg>(_nativeWindow);
	_windowSettingsDlgRef->hide();

	// object settings gui
	_objectSettingsDlgRef = std::make_shared<ObjectSettingsDlg>(_nativeWindow);
	_objectSettingsDlgRef->hide();

	// add object gui
	_objectInputerDlgRef = std::make_shared<ObjectInputerDlg>(_nativeWindow);
	_objectInputerDlgRef->hide();

	_bckColor = &_windowSettingsDlgRef->getBckColor();
	_windowSettingsDlgRef->getMonitorButtonSignal() = std::bind(&BuzzWindow::onStartMonitorProcess, this, _1);
	_objectInputerDlgRef->getAddObjectButtonSignal() = std::bind(&BuzzWindow::onAddObjectClick, this, _1);	
	
	// setup user's event
	_nativeWindow->getSignalKeyDown().connect(std::bind(&BuzzWindow::onKeyPress, this, _1));

	_nativeWindow->getSignalClose().connect(std::bind(&BuzzWindow::onClose, this));
	_nativeWindow->getSignalResize().connect(std::bind(&BuzzWindow::onResize, this));
	_nativeWindow->getSignalMouseWheel().connect(std::bind(&BuzzWindow::onMouseWheel, this, _1));

	_rootObject = make_shared<BuzzContainer>();
	_rootObject->setName("Sence");

	//onStartMonitorProcess(nullptr);
}

BuzzWindow& BuzzWindow::setTitle(const std::string& title) {
	_nativeWindow->setTitle(title);
	return *this;
}

BuzzWindow& BuzzWindow::setSize(int width, int height) {
	_nativeWindow->setSize(width, height);
	return *this;
}

void BuzzWindow::needUpdate(bool flag) {
	_needUpdate = flag;
}

void BuzzWindow::pendingUpdate() {

}

void applyObjectSettingForAll(BuzzDrawObj* obj, const std::function<void(BuzzDrawObj* obj)>& f) {
	f(obj);

	auto pContainer = dynamic_cast<BuzzContainer*>(obj);
	if (pContainer != nullptr) {
		auto& children = pContainer->getChildren();
		for (auto &aChildRef : children) {
			applyObjectSettingForAll(aChildRef.get(), f);
		}
	}
}

void applyLineColorForAll(BuzzDrawObj* obj, const ColorA& color) {
	int a, r, g, b;
	a = (int)(color.a * 255);
	r = (int)(color.r * 255);
	g = (int)(color.g * 255);
	b = (int)(color.b * 255);
	BuzzColor buzzColor = BUZZCOLOR(r, g, b, a);

	auto changeColor = [&buzzColor](BuzzDrawObj* obj) {
		obj->setColor(buzzColor);
	};

	applyObjectSettingForAll(obj, changeColor);
}

void BuzzWindow::draw() {
	gl::clear(*_bckColor);
	if (_pendingUpdate || _pendingResize) {
		double currentTimeFromLaunch = App::get()->getElapsedSeconds();
		if (currentTimeFromLaunch - _lastEventOccur > (double)_updateFBOIV) {
			// update the sence later
			needUpdate();

			if (_pendingResize) {
				auto size = _nativeWindow->getSize();
				_frameBuffer = gl::Fbo::create(size.x, size.y, true, true, true);
				_pendingResize = false;
			}
		}
	}

	if (_objectSettingsDlgRef->isVisible()) {
		if (_objectLine != _objectSettingsDlgRef->getObjectLineColor()) {
			_objectLine = _objectSettingsDlgRef->getObjectLineColor();
			applyLineColorForAll(_rootObject.get(), _objectLine);
			_needUpdate = true;
		}
		if (_objectLineWidth != _objectSettingsDlgRef->getLineWidth()) {
			_objectLineWidth = _objectSettingsDlgRef->getLineWidth();
			
			auto changeLineWidth = [this](BuzzDrawObj* obj) {
				auto lineObj = dynamic_cast<BuzzHasLine*>(obj);
				if (lineObj) {
					lineObj->setLineWidth((float)_objectLineWidth);
				}
			};

			applyObjectSettingForAll(_rootObject.get(), changeLineWidth);

			_needUpdate = true;
		}
	}

	if(_needUpdate)
	{		
		gl::ScopedFramebuffer fbScp(_frameBuffer);
		gl::ScopedModelMatrix scopedModelMatrix;
		gl::setModelMatrix(_transform);
		gl::clear(*_bckColor);
		if (_tex) {
			gl::draw(_tex);
		}

		_rootObject->draw();

		// reset update flag
		_needUpdate = _afterUpdate;
		// reset pending update flag
		_pendingUpdate = false;
	}

	if(_pendingUpdate)
	{
		gl::ScopedModelMatrix scopedModelMatrix;
		gl::setModelMatrix(_texTransform * _transform);
		gl::draw(_frameBuffer->getColorTexture());
	}
	else {
		gl::draw(_frameBuffer->getColorTexture());
	}
	pretzel::PretzelGui::drawAll();
}

void BuzzWindow::addNewObject(BuzzDrawObj* obj) {
	_rootObject->addObject(BuzzDrawObjRef(obj));
}

void BuzzWindow::showInputerWithAddress(void* address) {	
	_objectInputerDlgRef->setObjectAddress(address);
	_objectInputerDlgRef->show();
}

void BuzzWindow::onKeyPress(KeyEvent& e) {
	using namespace std::placeholders;

	if (!_activeDialog || _activeDialog->isVisible() == false) {
		if (e.getCode() == KeyEvent::KEY_w) {
			_needUpdate = true;
			_afterUpdate = true;
			_windowSettingsDlgRef->show();
			_activeDialog = _windowSettingsDlgRef;
			e.setHandled();
			_windowSettingsDlgRef->setCloseEventHandler([this](BuzzDialog*) {_afterUpdate = false; });
		}
		else if (e.getCode() == KeyEvent::KEY_e) {
			_objectSettingsDlgRef->show();
			_activeDialog = _objectSettingsDlgRef;
			e.setHandled();
		}
		else if (e.getCode() == KeyEvent::KEY_r) {
			_objectInputerDlgRef->show();
			_activeDialog = _objectInputerDlgRef;
			e.setHandled();
		}
		else if (e.getCode() == KeyEvent::KEY_t) {
			_hierarchyDialog = std::make_shared<ObjectHierarchyDlg>((HWND)_nativeWindow->getNative());
			_hierarchyDialog->setObjectRoot(_rootObject);
			_hierarchyDialog->setItemCheckChangedHandler(std::bind(&BuzzWindow::onObjectCheckedChanged, this, _1, _2));
			_hierarchyDialog->show();
		}
		else if (e.getCode() == KeyEvent::KEY_y) {
			_hierarchyDialog2 = std::make_shared<ObjectHierarchyDlg2>((HWND)_nativeWindow->getNative());
			_hierarchyDialog2->setObjectRoot(_rootObject);
			_hierarchyDialog2->setItemCheckChangedHandler(std::bind(&BuzzWindow::onObjectCheckedChanged, this, _1, _2));
			_hierarchyDialog2->show();
		}
	}
}

void BuzzWindow::onClose() {
	auto clipboardViewer = ClipboardViewer::getInstance();

	// check if current viewer is going to closed
	if (clipboardViewer->getCurrentViewer() != (HWND)_nativeWindow->getNative()) {
		return;
	}
	clipboardViewer->stopMonitor();

	// start monitor clipboard on another window
	auto app = App::get();
	auto windowCount = app->getNumWindows();
	if (windowCount < 2) {
		// no window left
		return;
	}

	// find another window available
	for (int i = (int)(windowCount - 1); i >= 0; i--) {
		auto nativeWindow = app->getWindowIndex(i);
		if (nativeWindow.get() != _nativeWindow.get()) {
			clipboardViewer->startMonitor((HWND)nativeWindow->getNative());
			break;
		}
	}
}

void BuzzWindow::onResize() {
	auto size =_nativeWindow->getSize();
	if (!_frameBuffer) {
		_frameBuffer = gl::Fbo::create(size.x, size.y, true, true, true);
		// need update FBO on draw method
		needUpdate();
	}
	else {
		_lastEventOccur = App::get()->getElapsedSeconds();
		_pendingResize = true;
	}
}

void BuzzWindow::onMouseWheel(MouseEvent& me) {
	float increment = me.getWheelIncrement();
	float ratio = 0.05f;
	ratio = 1.0f + increment * ratio;

	me.setHandled();

	if (_pendingUpdate == false) {
		_texTransform = glm::inverse(_transform);
	}

	_transform *= glm::scale(vec3(ratio, ratio, 1.0f));

	_lastEventOccur = App::get()->getElapsedSeconds();
	_pendingUpdate = true;
}

void BuzzWindow::onStartMonitorProcess(BuzzDialog* sender) {
	if (!_spyClient) {
		_spyClient = std::make_shared<BuzzSpyClient>();
	}

	auto& currentMonitorProcessName = _spyClient->getProcessName();
	auto desireProcessName = _windowSettingsDlgRef->getProcessName();

	if (desireProcessName.empty()) {
		return;
	}

	if (desireProcessName != currentMonitorProcessName) {
		_spyClient->stopMonitorProcess();
		_spyClient->startMonitorProcess(desireProcessName.c_str());
	}
	else if (_spyClient->checkTargetAvaible() == false) {
		_spyClient->startMonitorProcess(desireProcessName.c_str());
	}
}

void BuzzWindow::onAddObjectClick(BuzzDialog* sender) {	
	void* desireReadObjectAddress = _objectInputerDlgRef->getObjectAddress();
	int type = _objectInputerDlgRef->getSelectedTypeIndex();

	BuzzDrawObj* obj = readObject(desireReadObjectAddress, type);
	if (obj) {
		std::stringstream ss;
		ss << std::setfill('0') << std::setw(sizeof(void*) * 2)
			<< std::hex << desireReadObjectAddress;
		obj->setName(ss.str());
		addNewObject(obj);
	}
}

void BuzzWindow::onObjectCheckedChanged(BuzzDrawObj* obj, BOOL newState) {
	if (newState == TRUE) {
		obj->setVisible(true);
	}
	else if (newState == FALSE) {
		obj->setVisible(false);
	}
	needUpdate();
}