#pragma once
#include "pretzel/PretzelGui.h"

class BuzzCustomGui : public pretzel::PretzelGui {
public:
	BuzzCustomGui(const std::string& title, int width, int height, ci::app::WindowRef window);
	void keyDown(const char &key, const int &keyCode) override;
};