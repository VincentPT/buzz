#pragma once
#include "BuzzDrawObj.h"
#include <vector>

class BuzzContainer : public BuzzDrawObj {
protected:
	std::vector<BuzzDrawObjRef> _children;
public:
	BuzzContainer();
	virtual ~BuzzContainer();

	virtual BuzzLocation getLocation();
	virtual int addObject(BuzzDrawObjRef buzzDrawObjRef);

	// this function will be call each frame
	virtual void draw();
};