#include "BuzzSpyClient.h"
#include "ScopeAutoFunction.hpp"

#include <filesystem>
#include <string>

using namespace std;

BuzzSpyClient::BuzzSpyClient() {
}

BuzzSpyClient::~BuzzSpyClient() {

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

	string rootSpyPath = processParentPath + "\\spy.dll";
	list<string> dependencies = { processParentPath + "\\spylib.dll" };

	bool blRes = SpyClient::startMonitorProcess(processName, rootSpyPath, dependencies);
	return blRes;
}

int BuzzSpyClient::readCVMat(void* address, const std::function<void(ImageRawData*&)>& handler) {

	int nParam = 1;
	CustomCommandCmdData customData;
	customData.commandSize = sizeof(CustomCommandCmdData) - sizeof(CustomCommandCmdData::params) + nParam * sizeof(void*);
	customData.commandId = CommandId::CUSTOM_COMMAND;
	customData.customCommandId = CustomCommandId::OPENCV_READ_MAT_OBJECT;
	customData.paramCount = nParam;
	customData.params[0] = address;

	ReturnData& returnData = customData.returnData;
	int iRes;
	iRes = sendCommandToRemoteThread((BaseCmdData*)&customData, &returnData);
	if (iRes != 0) {
		return iRes;
	}

	ImageRawData* imgRawData;
	iRes = readCustomCommandResult(&returnData, (void**)&imgRawData);
	if (iRes != 0) {
		return iRes;
	}

	handler(imgRawData);

	if (imgRawData != nullptr) {
		free(imgRawData);
	}

	int freeBufferRes = freeCustomCommandResult(&returnData);
	return iRes;
}