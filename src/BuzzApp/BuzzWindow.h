#pragma once
#include <string>
#include "cinder/app/Renderer.h"

class BuzzWindow
{
	ci::app::WindowRef _nativeWindow;

protected:
	void setupWindow();
public:
	BuzzWindow(const std::string& title, int width, int height);
	BuzzWindow(ci::app::WindowRef nativeWindow);
	~BuzzWindow();

	BuzzWindow& setTitle(const std::string& title);
	BuzzWindow& setSize(int width, int height);
};

typedef std::shared_ptr<BuzzWindow> BuzzWindowRef;
