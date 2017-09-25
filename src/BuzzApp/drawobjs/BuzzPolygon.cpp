#include "BuzzPolygon.h"

#include <cinder\gl\gl.h>

using namespace ci;

BuzzPolygon::BuzzPolygon() {}
BuzzPolygon::~BuzzPolygon() {}

BuzzLocation BuzzPolygon::getLocation() {
	return BuzzLocation(0, 0);
}

int BuzzPolygon::addPoint(const float&x, const float&y) {
	_points.push_back(BuzzLocation(x, y));
	return (int)_points.size();
}

int BuzzPolygon::getPointCount() const {
	return (int)_points.size();
}

// this function will be call each frame
void BuzzPolygon::draw() {
	if (isVisible() == false) {
		return;
	}

	gl::ScopedColor scopeColor(BCRED(_rgba) / 255.0f, BCGREEN(_rgba) / 255.0f, BCBLUE(_rgba) / 255.0f, BCALPHA(_rgba) / 255.0f);
	gl::ScopedLineWidth scopeLineWidth(_lineWidth);
	
	// We're going to draw a line through all the points in the list
	// using a few convenience functions: 'begin' will tell OpenGL to
	// start constructing a line strip, 'vertex' will add a point to the
	// line loop and 'end' will execute the draw calls on the GPU.
	gl::begin(GL_LINE_LOOP);
	for (auto &point : _points) {
		vec2 p(point.x, point.y);
		gl::vertex(p);
	}
	gl::end();
}