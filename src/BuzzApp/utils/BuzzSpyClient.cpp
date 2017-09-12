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

template <class T>
int readCustomObject(BuzzSpyClient* buzzSpyClient, void* address, CustomCommandId customCmdId, const std::function<void(T*&)>& handler) {
	int nParam = 1;
	CustomCommandCmdData customData;
	customData.commandSize = sizeof(CustomCommandCmdData) - sizeof(CustomCommandCmdData::params) + nParam * sizeof(void*);
	customData.commandId = CommandId::CUSTOM_COMMAND;
	customData.customCommandId = customCmdId;
	customData.paramCount = nParam;
	customData.params[0] = address;

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
	return readCustomObject(this, address, CustomCommandId::OPENCV_READ_MAT_OBJECT, handler);
}

int BuzzSpyClient::readCVPoint(void* address, const std::function<void(PointRawData*&)>& handler) {
	return readCustomObject(this, address, CustomCommandId::OPENCV_READ_CVPOINT_OBJECT, handler);
}

int BuzzSpyClient::readCVPoint2f(void* address, const std::function<void(Point2fRawData*&)>& handler) {
	return readCustomObject(this, address, CustomCommandId::OPENCV_READ_CVPOINT2F_OBJECT, handler);
}

int BuzzSpyClient::readCVRect(void* address, const std::function<void(RectRawData*&)>& handler) {
	return readCustomObject(this, address, CustomCommandId::OPENCV_READ_CVRECT_OBJECT, handler);
}

int BuzzSpyClient::readCVContour(void* address, const std::function<void(PointArrayRawData*&)>& handler) {
	return readCustomObject(this, address, CustomCommandId::OPENCV_READ_CVCONTOUR, handler);
}

int BuzzSpyClient::readCVContours(void* address, const std::function<void(PointsArrayRawData*&)>& handler) {
	return readCustomObject(this, address, CustomCommandId::OPENCV_READ_CVCONTOURS, handler);
}