#include "BuzzDrawObj.h"

BuzzDrawObj::BuzzDrawObj() : _rgba(0x000000FF), _isVisible(true) {

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

const std::string& BuzzDrawObj::getName() const {
	return _name;
}

void BuzzDrawObj::setName(const std::string& name) {
	_name = name;
}

void BuzzDrawObj::setVisible(bool visible) {
	_isVisible = visible;
}

bool BuzzDrawObj::isVisible() const {
	return _isVisible;
}