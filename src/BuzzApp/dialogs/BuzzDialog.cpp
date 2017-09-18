#include "BuzzDialog.h"
#include "BuzzCustomGui.h"

BuzzDialog::BuzzDialog(const std::string& title, int width, int height, ci::app::WindowRef window) {
	_nativeDlg = std::make_shared<BuzzCustomGui> (title, width, height, window);

	_nativeDlg->setCloseEventHandler([this]() {
		if (_closeEventHandler) {
			_closeEventHandler(this);
		}
	});
}

BuzzDialog::~BuzzDialog() {}

BuzzCustomGuiRef BuzzDialog::getNative() const {
	return _nativeDlg;
}

void BuzzDialog::onClose() {
	if (_nativeDlg) {
		_nativeDlg->setVisible(false);
		if (_closeEventHandler) {
			_closeEventHandler(this);
		}
	}
}

void BuzzDialog::show() {
	if (_nativeDlg) {
		_nativeDlg->setVisible(true);
	}
}

void BuzzDialog::hide() {
	if (_nativeDlg) {
		_nativeDlg->setVisible(false);
	}
}

void BuzzDialog::display() {
	if (_nativeDlg && _nativeDlg->isVisible()) {
		_nativeDlg->draw();
	}
}

bool BuzzDialog::isVisible() {
	return (_nativeDlg && _nativeDlg->isVisible());
}

void BuzzDialog::setCloseEventHandler(DialogCloseEventHandler&& eventHandler) {
	_closeEventHandler = eventHandler;
}