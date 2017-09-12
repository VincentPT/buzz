#include "BuzzPoint.h"

#include <cinder\gl\gl.h>

using namespace ci;

BuzzPoint::BuzzPoint() : _location(0, 0) {
}

BuzzPoint::BuzzPoint(const float& x, const float& y) : _location(x,y) {
}

BuzzPoint::~BuzzPoint() {

}

BuzzLocation BuzzPoint::getLocation() {
	return _location;
}

const BuzzLocation& BuzzPoint::getLocation() const {
	return _location;
}

void BuzzPoint::draw() {
	constexpr float radius = 3;
	gl::ScopedColor scopeColor(BCRED(_rgba) / 255.0f, BCGREEN(_rgba) / 255.0f, BCBLUE(_rgba) / 255.0f, BCALPHA(_rgba) / 255.0f);
	gl::drawSolidCircle(glm::vec2(_location.x, _location.y), radius, 8);
}