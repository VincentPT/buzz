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

template <class T, class ...Args>
int readCustomObject(BuzzSpyClient* buzzSpyClient, CustomCommandId customCmdId, const std::function<void(T*&)>& handler, Args...args) {
	void* params[] = { args... };
	constexpr int nParam = sizeof...(args);
	char commandRaw[sizeof(CustomCommandCmdData) + (nParam - 1) * sizeof(void*)];

	CustomCommandCmdData& customData = *(CustomCommandCmdData*)&commandRaw[0];
	customData.commandSize = sizeof(commandRaw);
	customData.commandId = CommandId::CUSTOM_COMMAND;
	customData.customCommandId = customCmdId;
	customData.paramCount = nParam;

	memcpy_s(&customData.params[0], sizeof(params), &params[0], sizeof(params));

	ReturnData& returnData = customData.returnData;
	int iRes;
	iRes = buzzSpyClient->sendCommandToRemoteThread((BaseCmdData*)&customData, &returnData);
	if (iRes != 0) {
		return iRes;
	}

	T* rawData;
	iRes = buzzSpyClient->readCustomCommandResult(&returnData, (void**)&rawData);
	if (iRes != 0) {
		return iRes;
	}

	handler(rawData);

	if (rawData != nullptr) {
		free(rawData);
	}

	int freeBufferRes = buzzSpyClient->freeCustomCommandResult(&returnData);
	return iRes;
}

int BuzzSpyClient::readCVMat(void* address, const std::function<void(ImageRawData*&)>& handler) {
	return readCustomObject(this, CustomCommandId::OPENCV_READ_MAT_OBJECT, handler, address);
}

int BuzzSpyClient::readCVPoint(void* address, const std::function<void(PointRawData*&)>& handler) {
	return readCustomObject(this, CustomCommandId::OPENCV_READ_CVPOINT_OBJECT, handler, address);
}

int BuzzSpyClient::readCVPoint2f(void* address, const std::function<void(Point2fRawData*&)>& handler) {
	return readCustomObject(this, CustomCommandId::OPENCV_READ_CVPOINT2F_OBJECT, handler, address);
}

int BuzzSpyClient::readCVRect(void* address, const std::function<void(RectRawData*&)>& handler) {
	return readCustomObject(this, CustomCommandId::OPENCV_READ_CVRECT_OBJECT, handler, address);
}

int BuzzSpyClient::readCVContour(void* address, const std::function<void(PointArrayRawData*&)>& handler) {
	return readCustomObject(this, CustomCommandId::OPENCV_READ_CVCONTOUR, handler, address);
}

int BuzzSpyClient::readCVContours(void* address, SortContourMode sortMode, const std::function<void(PointsArrayRawData*&)>& handler) {
	return readCustomObject(this, CustomCommandId::OPENCV_READ_CVCONTOURS, handler, address, (void*)sortMode);
}