#pragma once
#include "cinder/app/app.h"
#include <functional>

class BuzzDialog;
typedef std::function<void(BuzzDialog* sender)> BuzzGeneralEventHandler;
typedef BuzzGeneralEventHandler ButtonClickEventHandler;
typedef BuzzGeneralEventHandler DialogCloseEventHandler;

class BuzzCustomGui;
typedef std::shared_ptr<BuzzCustomGui> BuzzCustomGuiRef;

class BuzzDialog {
	BuzzCustomGuiRef		_nativeDlg;
	DialogCloseEventHandler _closeEventHandler;
protected:
	BuzzCustomGuiRef getNative() const;
public:
	BuzzDialog(const std::string& title, int width, int height, ci::app::WindowRef window);
	virtual ~BuzzDialog();

	void onClose();
	virtual void show();
	virtual void hide();
	virtual void display();
	virtual bool isVisible();
	virtual void setCloseEventHandler(DialogCloseEventHandler&& eventHandler);
};