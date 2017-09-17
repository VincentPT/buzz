#pragma once
#include "BuzzDialog.h"
#include <string>
#include <vector>
#include "../spylib/spylib_interfaces.h"

class ObjectInputerDlg : public BuzzDialog {
	std::string					_objectAddress;
	std::vector<std::string>    _objectTypes;
	std::vector<std::string>    _sortTypes;
	int                         _objectTypeIdx = 0;
	int                         _sortTypeIdx = 4;
	ButtonClickEventHandler		_onAddObjectBtnClick;
private:
	void onAddObjectButtonPress();
public:
	ObjectInputerDlg(ci::app::WindowRef window);
	~ObjectInputerDlg();
	int getSelectedTypeIndex() const;
	SortContourMode getSortType() const;
	ObjectInputerDlg& setSelectedTypeIndex(int idx);
	void* getObjectAddress() const;
	ObjectInputerDlg& setObjectAddress(void* address);

	ButtonClickEventHandler& getAddObjectButtonSignal();

public:
	static void* convertToAddress(const std::string& address);
};