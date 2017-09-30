#include "BuzzSpyClient.h"
#include "SpyClientUtils.hpp"

#include <filesystem>
#include <string>
#include <iostream>

using namespace std;

#define SPY_LIB_NAME "spylib.dll"

BuzzSpyClient::BuzzSpyClient() : _predefinedBase(CUSTOM_COMMAND_END), _hSpyLib(INVALID_MODULE_ID) {
}

BuzzSpyClient::~BuzzSpyClient() {

}

void BuzzSpyClient::setPredefinedCommandIdBase(int predefinedBase) {
	_predefinedBase = (CustomCommandId)predefinedBase;
}

bool BuzzSpyClient::loadSpyLib() {
	cout << "loading spy lib " SPY_LIB_NAME << endl;

	int ret = loadPredefinedFunctions(SPY_LIB_NAME, &_hSpyLib);
	int loadedFunctionCount = GET_NUMBER_OF_LOAD_PREDEFINED_FUNC(ret);
	CustomCommandId commandBase = GET_BASE_OF_LOAD_PREDEFINED_FUNC(ret);

	// print the spy results
	cout << "number of loaded function " << loadedFunctionCount << endl;
	cout << "custom commands begin at " << commandBase << endl;

	// store command id base and use it to invoke inside the UserSpyClient methods
	setPredefinedCommandIdBase(commandBase);

	bool res = (loadedFunctionCount > 0 && commandBase != CUSTOM_COMMAND_END);

	if (res) {
		CustomCommandId cmid;		
		auto cmdIdEnd = (CustomCommandId)UserCommandId::PRE_DEFINED_COMMAND_COUNT;
		for (cmid = 0; cmid < cmdIdEnd; cmid++) {
			getFunctionPtr(cmid + commandBase, &_checkFunctionPtrs[cmid]);
		}
	}

	return res;
}

void BuzzSpyClient::unloadSpyLib() {
	if (_hSpyLib != INVALID_MODULE_ID) {
		if (unloadModule(_hSpyLib) != 0) {
			cout << "unload spy lib " SPY_LIB_NAME " failed!" << endl;
		}
		else {
			_hSpyLib = INVALID_MODULE_ID;
		}
	}
}

bool BuzzSpyClient::startMonitorProcess(const char* processName) {
	auto hCurrentProcessBase = GetModuleHandleA(NULL);
	string currentProcesssFilePath(256, ' ');

	if (GetModuleFileNameA(hCurrentProcessBase, &currentProcesssFilePath[0], (DWORD)currentProcesssFilePath.size()) == FALSE) {
		return false;
	}
	
	using namespace std::experimental::filesystem::v1;
	path thePath(currentProcesssFilePath);
	string processParentPath = thePath.parent_path().u8string();

	string rootSpyPath = processParentPath + "\\" SPY_ROOT_DLL_NAME;
	list<string> dependencies = {
	};

	bool blRes = SpyClient::inject(processName, rootSpyPath, dependencies);
	if (blRes) {
		blRes = loadSpyLib();
	}

	return blRes;
}

bool BuzzSpyClient::stopMonitorProcess() {
	unloadSpyLib();
	bool res = uninject();
	if (!res) {
		cout << "stopMonitorProcess failed!" << endl;
	}

	return res;
}

bool BuzzSpyClient::restartMonitorProcess() {
	stopMonitorProcess();
	auto blRes = reinject();
	if (blRes) {
		blRes = loadSpyLib();
	}

	return blRes;
}

bool BuzzSpyClient::checkCommandsReady() {
	bool res = checkTargetAvaible();
	if (res == false) {
		res = restartMonitorProcess();
		if (res) {
			cout << "Restarted monitor process" << endl;
		}
	}
	else {
		list<CustomCommandId> cmdIds;
		int iRes = getModuleData(_hSpyLib, cmdIds, nullptr);
		bool needReload = false;

		// check if error occurs...
		needReload = iRes != 0;

		if (needReload == false) {
			// make sure that the order of return command is same with current predefined command
			cmdIds.sort();

			auto it = cmdIds.begin();
			CustomCommandId cmid;
			auto cmdIdEnd = (CustomCommandId)UserCommandId::PRE_DEFINED_COMMAND_COUNT + _predefinedBase;
			for (cmid = _predefinedBase; cmid < cmdIdEnd; cmid++, it++) {
				if (cmid != *it) {
					break;
				}				
			}

			if (cmid != cmdIdEnd || it != cmdIds.end()) {
				needReload = true;
			}
		}
		if (needReload == false) {
			CustomCommandId cmid;
			void* ptr;
			auto cmdIdEnd = (CustomCommandId)UserCommandId::PRE_DEFINED_COMMAND_COUNT;
			for (cmid = 0; cmid < cmdIdEnd; cmid++) {
				getFunctionPtr(cmid + _predefinedBase, &ptr);
				if (ptr != _checkFunctionPtrs[cmid]) {
					needReload = true;
					break;
				}
			}
		}

		if (needReload) {
			unloadSpyLib();
			res = loadSpyLib();
			if (res) {
				cout << "Reloaded spy lib" << endl;
			}
		}
	}

	return res;
}

template <class T, class ...Args>
int readCustomObject(BuzzSpyClient* buzzSpyClient, CustomCommandId customCmdId, const std::function<void(T&)>& handler, Args...args) {

	auto nativeHandler = [&handler](ReturnData& returnData) {
		T customdata = (T)returnData.customData;
		handler(customdata);
		returnData.customData = (char*)customdata;
	};

	return executeCommandAndFreeCustomData(buzzSpyClient, customCmdId, nativeHandler, args...);
}

int BuzzSpyClient::readCVMat(void* address, const std::function<void(ImageRawData*&)>& handler) {
	return readCustomObject(this, (CustomCommandId)UserCommandId::OPENCV_READ_MAT_OBJECT + _predefinedBase, handler, address);
}

int BuzzSpyClient::readCVPoint(void* address, const std::function<void(PointRawData*&)>& handler) {
	return readCustomObject(this, (CustomCommandId)UserCommandId::OPENCV_READ_CVPOINT_OBJECT + _predefinedBase, handler, address);
}

int BuzzSpyClient::readCVPoint2f(void* address, const std::function<void(Point2fRawData*&)>& handler) {
	return readCustomObject(this, (CustomCommandId)UserCommandId::OPENCV_READ_CVPOINT2F_OBJECT + _predefinedBase, handler, address);
}

int BuzzSpyClient::readCVRect(void* address, const std::function<void(RectRawData*&)>& handler) {
	return readCustomObject(this, (CustomCommandId)UserCommandId::OPENCV_READ_CVRECT_OBJECT + _predefinedBase, handler, address);
}

int BuzzSpyClient::readCVContour(void* address, const std::function<void(PointArrayRawData*&)>& handler) {
	return readCustomObject(this, (CustomCommandId)UserCommandId::OPENCV_READ_CVCONTOUR + _predefinedBase, handler, address);
}

int BuzzSpyClient::readCVContours(void* address, SortContourMode sortMode, const std::function<void(PointsArrayRawData*&)>& handler) {
	return readCustomObject(this, (CustomCommandId)UserCommandId::OPENCV_READ_CVCONTOURS + _predefinedBase, handler, address, (void*)sortMode);
}

int BuzzSpyClient::readDummyTree(void* address, const std::function<void(char*&)>& handler) {
	return readCustomObject(this, (CustomCommandId)UserCommandId::READ_DUMMYTREE + _predefinedBase, handler, address);
}