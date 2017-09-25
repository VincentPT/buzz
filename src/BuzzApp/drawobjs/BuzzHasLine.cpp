#include "BuzzHasLine.h"

BuzzHasLine::BuzzHasLine() : _lineWidth(1) {
}

BuzzHasLine::~BuzzHasLine() {}

void BuzzHasLine::setLineWidth(float lineWidth) {
	_lineWidth = lineWidth;
}

float BuzzHasLine::getLineWidth() const {
	return _lineWidth;
}