#pragma once
#include "BuzzDialog.h"
#include <string>
#include <vector>

class ObjectInputerDlg : public BuzzDialog {
	std::string					_objectAddress;
	std::vector<std::string>    _objectTypes;
	int                         _objectTypeIdx = 0;
private:
	void onAddObjectButtonPress();
public:
	ObjectInputerDlg(ci::app::WindowRef window);
	~ObjectInputerDlg();
	int getSelectedTypeIndex() const;
	ObjectInputerDlg& setSelectedTypeIndex(int idx);
};