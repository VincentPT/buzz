#include "BuzzDialog.h"
#include "BuzzCustomGui.h"

BuzzDialog::BuzzDialog(const std::string& title, int width, int height, ci::app::WindowRef window) {
	_nativeDlg = std::make_shared<BuzzCustomGui> (title, width, height, window);
}

BuzzDialog::~BuzzDialog() {}

BuzzCustomGuiRef BuzzDialog::getNative() const {
	return _nativeDlg;
}

void BuzzDialog::onClose() {
	if (_nativeDlg) {
		_nativeDlg->setVisible(false);
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