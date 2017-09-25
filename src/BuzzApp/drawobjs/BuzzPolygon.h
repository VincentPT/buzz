#pragma once
#include "BuzzHasLine.h"
#include <vector>

class BuzzPolygon : public BuzzHasLine {
protected:
	std::vector<BuzzLocation> _points;
public:
	BuzzPolygon();
	virtual ~BuzzPolygon();

	virtual BuzzLocation getLocation();
	virtual int addPoint(const float&x, const float&y);
	virtual int getPointCount() const;

	// this function will be call each frame
	virtual void draw();
};