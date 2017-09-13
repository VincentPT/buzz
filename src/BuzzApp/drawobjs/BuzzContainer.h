#pragma once
#include "BuzzDrawObj.h"
#include <list>

class BuzzContainer : public BuzzDrawObj {
protected:
	std::list<BuzzDrawObjRef> _children;
public:
	BuzzContainer();
	virtual ~BuzzContainer();

	virtual BuzzLocation getLocation();
	virtual int addObject(BuzzDrawObjRef buzzDrawObjRef);
	const std::list<BuzzDrawObjRef>& getChildren() const;

	// this function will be call each frame
	virtual void draw();
};