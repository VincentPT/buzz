#include "ObjectInputerDlg.h"
#include "BuzzCustomGui.h"

using namespace ci;
using namespace ci::app;

ObjectInputerDlg::ObjectInputerDlg(ci::app::WindowRef window) :
	BuzzDialog("Add object", 300, 500, window) {
	pretzel::PretzelGuiRef nativeDlg = getNative();
	_objectTypes = { "OpenCV Mat", "OpenCV Rect", "OpenCV Contour" };
	nativeDlg->addTextField("Address", &_objectAddress, true);
	nativeDlg->addEnum("Object type", &_objectTypes, &_objectTypeIdx);
	nativeDlg->addButton("Add", &ObjectInputerDlg::onAddObjectButtonPress, this);
	nativeDlg->addButton("Close", &BuzzDialog::onClose, (BuzzDialog*)this);
}

ObjectInputerDlg::~ObjectInputerDlg() {}

int ObjectInputerDlg::getSelectedTypeIndex() const {
	return _objectTypeIdx;
}
ObjectInputerDlg& ObjectInputerDlg::setSelectedTypeIndex(int idx) {
	_objectTypeIdx = idx;
	return *this;
}

void ObjectInputerDlg::onAddObjectButtonPress() {

}