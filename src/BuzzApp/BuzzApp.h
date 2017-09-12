#pragma once
#include "cinder/app/App.h"
#include "BuzzWindow.h"

#include <list>

// We'll create a new Cinder Application by deriving from the App class
class BuzzApp : public ci::app::App {
public:
	BuzzApp();
	~BuzzApp();
	void setup();

	void mouseDrag(ci::app::MouseEvent event);
	void keyDown(ci::app::KeyEvent event);
	void draw();
	void update();
	void createNewWindow();
	void onClipboardTextChanged(const std::string& text);
};
