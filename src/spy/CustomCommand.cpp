#include <Shlwapi.h>

#include "CustomCommand.h"
#include "Spy.h"
#include "CustomCommandInvoker.h"

#include "../spylib/OpenCVObjectsCommands.h"

#include <iostream>
#include <vector>
#include <list>

// using namespaces
using namespace std;

// extern variables declaration
extern int _runner_count;

struct FunctionInfo
{
	void* address;
	bool available;
};

// global variables
bool g_predefinedFuncLoaded = false;
vector<FunctionInfo> _custom_funcs((size_t)CustomCommandId::PRE_DEFINED_COMMAND_COUNT, {0});

// macro definition
#define IS_CUSTOMFUNCTION_REACH_LIMIT() (_custom_funcs.size() >= (size_t)CustomCommandId::CUSTOM_COMMAND_END - 1)

inline CustomCommandId addCustomFunction(void* pFunc) {
	if (IS_CUSTOMFUNCTION_REACH_LIMIT()) {
		cout << "The custom library reachs maximum number of function" << std::endl;
		return CustomCommandId::CUSTOM_COMMAND_END;
	}

	FunctionInfo functionInfo = { pFunc, true };
	_custom_funcs.push_back(functionInfo);
	return (CustomCommandId)(_custom_funcs.size());
}

inline bool setCustomFunction(CustomCommandId cmdId, void* pFunc) {
	if ((size_t)cmdId >= _custom_funcs.size()) {
		cout << "Out of range" << std::endl;
		return false;
	}
	FunctionInfo functionInfo = { pFunc, true };
	_custom_funcs[(size_t)cmdId] = functionInfo;
	return true;
}

inline void* getCustomFunction(CustomCommandId cmdId) {
	if ((size_t)cmdId >= _custom_funcs.size()) {
		cout << "Out of range" << std::endl;
		return nullptr;
	}
	FunctionInfo& functionInfo = _custom_funcs[(size_t)cmdId];
	if (!functionInfo.available) {
		return nullptr;
	}
	return functionInfo.address;
}


// load custom functions function
// all custom functions must be set here
void loadCustomFunctions() {
	if (g_predefinedFuncLoaded) {
		return;
	}
	setCustomFunction(CustomCommandId::OPENCV_READ_MAT_OBJECT, readCVMatObject);
	setCustomFunction(CustomCommandId::OPENCV_READ_CVPOINT_OBJECT, readCVPointObject);
	setCustomFunction(CustomCommandId::OPENCV_READ_CVPOINT2F_OBJECT, readCVPointFObject);
	setCustomFunction(CustomCommandId::OPENCV_READ_CVRECT_OBJECT, readCVRectObject);
	setCustomFunction(CustomCommandId::OPENCV_READ_CVCONTOUR, readCVContour);
	setCustomFunction(CustomCommandId::OPENCV_READ_CVCONTOURS, readCVContours);
	setCustomFunction(CustomCommandId::READ_DUMMYTREE, readDummyTree);

	g_predefinedFuncLoaded = true;
}

// this function use to invoke the corresponding custom function with the custom data
int invokeCustomCommand(CustomCommandCmdData* commandData) {
	if (commandData->commandSize != sizeof(CustomCommandCmdData) + (commandData->paramCount - 1) * sizeof(void*)) {
		cout << "Invalid custom command:" << commandData->commandSize << std::endl;
		return -1;
	}

	// load custom function before call
	loadCustomFunctions();

	// get function pointer
	void* fx = getCustomFunction(commandData->customCommandId);
	if (fx == nullptr) {
		cout << "custom command '" << (unsigned short)commandData->customCommandId << "' has been not set" << std::endl;
		return -1;
	}

	if (_runner_count <= commandData->paramCount) {
		cout << "custom command " << commandData->paramCount << " parameters has been not supported" << std::endl;
		return -1;
	}

	CustomCommandInvoker invoker(fx, commandData->paramCount);
	commandData->returnData = invoker.invoke(commandData->params);
	return 0;
}

int LoadCustomFunctions(LoadCustomFunctionsCmdData* commandData) {
	if (IS_CUSTOMFUNCTION_REACH_LIMIT()) {
		cout << "The custom library reachs maximum number of function" << std::endl;
		return -1;
	}

	if (commandData->commandSize < sizeof(LoadCustomFunctionsCmdData)) {
		cout << "Invalid load custom command" << std::endl;
		return -1;
	}

	int bufferSize = commandData->commandSize - sizeof(LoadCustomFunctionsCmdData) + sizeof(LoadCustomFunctionsCmdData::fNames);
	if (commandData->fNames[bufferSize - 1] != 0) {
		cout << "Invalid load custom command" << std::endl;
		return -1;
	}
	char* dllFile = commandData->fNames;
	char* endBuffer = dllFile + bufferSize;
	auto c = dllFile + strlen(dllFile) + 1;

	// first try load the dll file as it is, dll file can be an absolute path or a relative path
	HMODULE hModule = LoadLibrary(dllFile);
	vector<CustomCommandId> commandIds;
	commandData->returnData.sizeOfCustomData = 0;
	commandData->returnData.customData = nullptr;

	// if dll cannot be load...
	if (hModule == NULL) {
		// if the dll file is not a relative path, we don't process more
		if (!PathIsRelative(dllFile)) {
			return 0;
		}
		// if the dll file is a relative path, we find it in location of root dll
		HMODULE hModuleRoot = GetModuleHandle(SPY_ROOT_DLL_NAME);
		if (hModuleRoot == nullptr) {
			cout << "Name of spy root dll is changed." << std::endl;
			return -1;
		}
		char path[MAX_PATH];
		int pathLen;
		if (!(pathLen = GetModuleFileName(hModuleRoot, path, MAX_PATH))) {
			cout << "Cannot find location of root dll path" << std::endl;
			return -1;
		}

		// remove root dll name to get the directory
		path[pathLen - strlen(SPY_ROOT_DLL_NAME)] = 0;

		string moduleFullPath(path);
		moduleFullPath.append(dllFile);
		hModule = LoadLibrary(moduleFullPath.c_str());
		if (hModule == nullptr) {
			return 0;
		}
	}
	
	// load procs address from the loaded dll file
	void* pFunc;
	CustomCommandId customCommandId;
	while (c < endBuffer)
	{		
		while (c < endBuffer && (*c == ' ' || *c == '\t') || *c == 0) {
			c++;
		}
		if (c >= endBuffer) {
			break;
		}
		
		pFunc = GetProcAddress(hModule, c);
		if (pFunc) {
			customCommandId = addCustomFunction(pFunc);
		}
		else {
			customCommandId = CustomCommandId::CUSTOM_COMMAND_END;
		}

		commandIds.push_back(customCommandId);

		c += strlen(c) + 1;
	}
	ReturnData& returnData = commandData->returnData;
	returnData.sizeOfCustomData = (int)(sizeof(HMODULE) + commandIds.size() * sizeof(CustomCommandId));

	// allocate memory for return data, spy client should be responsible to free the memory after using
	char* rawData = (char*)malloc(returnData.sizeOfCustomData);
	returnData.customData = rawData;

	*((HMODULE*)rawData) = hModule;
	rawData += sizeof(HMODULE);
	memcpy_s(rawData, commandIds.size() * sizeof(CustomCommandId), commandIds.data(), commandIds.size() * sizeof(CustomCommandId));
	
	return 0;
}
