#pragma once
#include <memory>
#include <string>

// rgba color
typedef unsigned int BuzzColor;

#define BCRED(color) ((unsigned char)((color) >> 24))
#define BCGREEN(color) ((unsigned char)(((color) & 0x00FF0000) >> 16))
#define BCBLUE(color) ((unsigned char)(((color) & 0x0000FF00) >> 8))
#define BCALPHA(color) ((unsigned char)(color))
#define BUZZCOLOR(r,g,b,a) (BuzzColor)(((r) << 24) | ((g) << 16) | ((b) << 8) | a)

struct BuzzLocation {
	float x;
	float y;

	BuzzLocation(const float& x, const float& y) : x(x), y(y) {
	}
};

class BuzzDrawObj {
protected:
	BuzzColor _rgba;
	std::string _name;
	bool _isVisible;
public:
	BuzzDrawObj();
	virtual ~BuzzDrawObj();

	// this function will be call each frame before draw function is called
	virtual void update();

	// this function will be call each frame
	virtual void draw() = 0;

	virtual BuzzLocation getLocation() = 0;

	void setColor(const BuzzColor& rgba);
	const BuzzColor& getColor() const;

	const std::string& getName() const;
	void setName(const std::string&);

	void setVisible(bool visible);
	bool isVisible() const;
};

typedef std::shared_ptr<BuzzDrawObj> BuzzDrawObjRef;