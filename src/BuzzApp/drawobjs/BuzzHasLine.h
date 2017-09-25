#pragma once
#include "BuzzDrawObj.h"

class BuzzHasLine : public BuzzDrawObj {
protected:
	float _lineWidth;
public:
	BuzzHasLine();
	virtual ~BuzzHasLine();
	void setLineWidth(float lineWidth);
	float getLineWidth() const;
};