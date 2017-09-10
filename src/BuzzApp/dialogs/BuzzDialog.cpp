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
	_nativeDlg->setVisible(false);
}

void BuzzDialog::show() {
	_nativeDlg->setVisible(true);
}

void BuzzDialog::hide() {
	_nativeDlg->setVisible(false);
}

void BuzzDialog::display() {
	_nativeDlg->draw();
}