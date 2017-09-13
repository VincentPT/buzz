#include "BuzzContainer.h"

#include <cinder\gl\gl.h>

using namespace ci;

BuzzContainer::BuzzContainer() {

}

BuzzContainer::~BuzzContainer() {

}

BuzzLocation BuzzContainer::getLocation() {
	return BuzzLocation(0, 0);
}

int BuzzContainer::addObject(BuzzDrawObjRef buzzDrawObjRef) {
	_children.push_back(buzzDrawObjRef);
	return (int)_children.size();
}

// this function will be call each frame
void BuzzContainer::draw() {
	if (isVisible() == false) {
		return;
	}
	for (BuzzDrawObjRef& obj : _children) {
		obj->draw();
	}
}

const std::list<BuzzDrawObjRef>& BuzzContainer::getChildren() const {
	return _children;
}