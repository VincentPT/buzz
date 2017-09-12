#include "BuzzDrawObj.h"

BuzzDrawObj::BuzzDrawObj() : _rgba(0x000000FF) {

}

BuzzDrawObj::~BuzzDrawObj() {

}

void BuzzDrawObj::update() {}

void BuzzDrawObj::setColor(const BuzzColor& rgba) {
	_rgba = rgba;
}

const BuzzColor& BuzzDrawObj::getColor() const {
	return _rgba;
}