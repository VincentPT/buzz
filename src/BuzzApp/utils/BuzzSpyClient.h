#pragma once
#include "SpyClient.h"
#include "spylib_interfaces.h"

#include <Windows.h>
#include <functional>

class BuzzSpyClient : public SpyClient {
	CustomCommandId _predefinedBase;
	void* _checkFunctionPtrs[(int)UserCommandId::PRE_DEFINED_COMMAND_COUNT];
	ModuleId _hSpyLib;
protected:
	void setPredefinedCommandIdBase(int predefinedBase);
	bool loadSpyLib();
	void unloadSpyLib();
public:
	BuzzSpyClient();
	~BuzzSpyClient();

	bool startMonitorProcess(const char* processName);
	bool stopMonitorProcess();
	bool restartMonitorProcess();
	bool checkCommandsReady();
	
	int readCVMat(void* address, const std::function<void(ImageRawData*&)>& handler);
	int readCVPoint(void* address, const std::function<void(PointRawData*&)>& handler);
	int readCVPoint2f(void* address, const std::function<void(Point2fRawData*&)>& handler);
	int readCVRect(void* address, const std::function<void(RectRawData*&)>& handler);
	int readCVContour(void* address, const std::function<void(PointArrayRawData*&)>& handler);
	int readCVContours(void* address, SortContourMode sortMode, const std::function<void(PointsArrayRawData*&)>& handler);
	int readDummyTree(void* address, const std::function<void(char*&)>& handler);
};