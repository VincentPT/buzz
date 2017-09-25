#include "BuzzRect.h"

#include <cinder\gl\gl.h>

using namespace ci;

BuzzRect::BuzzRect() : _location(0,0), _size(0,0) {
}

BuzzRect::BuzzRect(const float& x, const float& y, const float& w, const float& h) : _location(x,y), _size(w,h){

}

BuzzRect::~BuzzRect() {}

BuzzLocation BuzzRect::getLocation() {
	return _location;
}

const BuzzLocation& BuzzRect::getLocation() const {
	return _location;
}

const BuzzLocation& BuzzRect::getSize() const {
	return _size;
}

// this function will be call each frame
void BuzzRect::draw() {
	if (isVisible() == false) {
		return;
	}

	gl::ScopedColor scopeColor(BCRED(_rgba)/255.0f, BCGREEN(_rgba) / 255.0f, BCBLUE(_rgba) / 255.0f, BCALPHA(_rgba) / 255.0f);
	auto rect = ci::Rectf(_location.x, _location.y, _location.x + _size.x, _location.y + _size.y);
	gl::drawStrokedRect(rect, _lineWidth);
}