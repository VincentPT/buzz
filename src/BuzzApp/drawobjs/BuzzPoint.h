#pragma once
#include "BuzzDrawObj.h"

class BuzzPoint : public BuzzDrawObj {
protected:
	BuzzLocation _location;
public:
	BuzzPoint();
	BuzzPoint(const float& x, const float& y);
	virtual ~BuzzPoint();

	virtual BuzzLocation getLocation();
	const BuzzLocation& getLocation() const;

	// this function will be call each frame
	virtual void draw();
};