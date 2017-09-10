#pragma once
#include "BuzzDialog.h"
#include <string>
#include <vector>
//#include <functional>

#include "../spylib/spylib_interfaces.h"

class ObjectInputerDlg : public BuzzDialog {
	std::string					_objectAddress;
	std::vector<std::string>    _objectTypes;
	int                         _objectTypeIdx = 0;
	std::vector<std::string>    _pixelFormats;
	int                         _pixelFormatIdx = 1;
	//std::function<void(BuzzDialog*)> _addObjectSignal;
private:
	void onAddObjectButtonPress();
public:
	ObjectInputerDlg(ci::app::WindowRef window);
	~ObjectInputerDlg();
	int getSelectedTypeIndex() const;
	ObjectInputerDlg& setSelectedTypeIndex(int idx);
	RawDataPixelFormat getPixelFormat() const;
	ObjectInputerDlg& getSelectedPixelFormat(RawDataPixelFormat pixelFormat);
	void* getObjectAddress() const;
	ObjectInputerDlg& setObjectAddress(void* address);

public:
	static void* convertToAddress(const std::string& address);
};