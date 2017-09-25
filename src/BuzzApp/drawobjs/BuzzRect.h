#pragma once
#include "BuzzHasLine.h"

class BuzzRect : public BuzzHasLine {
protected:
	BuzzLocation _location;
	BuzzLocation _size;
public:
	BuzzRect();
	BuzzRect(const float& x, const float& y, const float& w, const float& h);
	virtual ~BuzzRect();

	virtual BuzzLocation getLocation();
	const BuzzLocation& getLocation() const;
	const BuzzLocation& getSize() const;

	// this function will be call each frame
	virtual void draw();
};