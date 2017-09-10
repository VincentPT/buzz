#pragma once
#include "cinder/app/app.h"

class BuzzCustomGui;
typedef std::shared_ptr<BuzzCustomGui> BuzzCustomGuiRef;

class BuzzDialog {
	BuzzCustomGuiRef		_nativeDlg;
protected:
	BuzzCustomGuiRef getNative() const;
public:
	BuzzDialog(const std::string& title, int width, int height, ci::app::WindowRef window);
	virtual ~BuzzDialog();

	void onClose();
	virtual void show();
	virtual void hide();
	virtual void display();
};