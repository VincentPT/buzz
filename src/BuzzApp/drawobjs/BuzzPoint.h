#pragma once
#include "BuzzDrawObj.h"

class BuzzPoint : public BuzzDrawObj {
protected:
	BuzzLocation _location;
	float _radius;
public:
	BuzzPoint();
	BuzzPoint(const float& x, const float& y);
	virtual ~BuzzPoint();

	virtual BuzzLocation getLocation();
	const BuzzLocation& getLocation() const;

	void setRadius(float radius);

	// this function will be call each frame
	virtual void draw();
};