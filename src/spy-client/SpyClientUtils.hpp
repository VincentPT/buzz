#pragma once
#include "spy_interfaces.h"
#include <functional>
#include <iostream>

class ScopeAutoFunction {
	std::function<void()> _fx;
public:
	
	ScopeAutoFunction(const std::function<void()>& fx) {
		_fx = fx;
	}
	
	ScopeAutoFunction(std::function<void()>&& fx) {
		_fx = fx;
	}

	~ScopeAutoFunction() {
		_fx();
	}
};

class SpyClient;


/// allocate buffer in remote process and copy val to allcated buffer
/// use must call freeRemoteBuffer to deallocate buffer after using.
template <class T>
void* cloneBufferToRemoteProcess(SpyClient* spyClient, const T& val) {
	void* remoteBuffer = spyClient->allocateRemoteBuffer(sizeof(T));
	if (remoteBuffer == nullptr) {
		return nullptr;
	}

	if (spyClient->writeDataToRemoteProcess(remoteBuffer, (void*)&val, sizeof(T))) {
		spyClient->freeRemoteBuffer(remoteBuffer);
		return nullptr;
	}

	return remoteBuffer;
}

template <class ...Args>
int executeCommandAndFreeCustomData(SpyClient* spyClient, CustomCommandId customCmdId, const std::function<void(ReturnData&)>& handler, Args...args) {
	constexpr int nParam = sizeof...(args);
	constexpr size_t paramSize = nParam * sizeof(void*);
	char commandRaw[sizeof(CustomCommandCmdData) + paramSize - sizeof(CustomCommandCmdData::params)];

	CustomCommandCmdData& customData = *(CustomCommandCmdData*)&commandRaw[0];
	customData.commandSize = sizeof(commandRaw);
	customData.commandId = CommandId::CUSTOM_COMMAND;
	customData.customCommandId = customCmdId;
	customData.paramCount = nParam;

	{
		void* params[] = { (void*)(size_t)args..., nullptr };
		memcpy_s(&customData.params[0], paramSize, &params[0], paramSize);
	}

	ReturnData& returnData = customData.returnData;
	int iRes;
	DWORD invokeResult;
	iRes = spyClient->sendCommandToRemoteThread((BaseCmdData*)&customData, true, &invokeResult);
	if (iRes != 0) {
		return iRes;
	}
	if (invokeResult != 0) {
		return invokeResult;
	}

	ReturnData localReturnData = returnData;

	iRes = spyClient->readCustomCommandResult(&returnData, (void**)&localReturnData.customData);
	if (iRes != 0) {
		return iRes;
	}

	handler(localReturnData);

	// free customData in local process
	if (localReturnData.customData != nullptr) {
		free(localReturnData.customData);
	}

	// free customData in remote process
	if (spyClient->freeCustomCommandResult(&returnData) != 0) {
		std::cout << "an error occurs when release custom data" << endl;
	}
	return iRes;
}


template <class ...Args>
int executeCommand(SpyClient* spyClient, CustomCommandId customCmdId, const std::function<void(ReturnData&)>& handler, Args...args) {
	constexpr int nParam = sizeof...(args);
	constexpr size_t paramSize = nParam * sizeof(void*);
	char commandRaw[sizeof(CustomCommandCmdData) + paramSize - sizeof(CustomCommandCmdData::params)];

	CustomCommandCmdData& customData = *(CustomCommandCmdData*)&commandRaw[0];
	customData.commandSize = sizeof(commandRaw);
	customData.commandId = CommandId::CUSTOM_COMMAND;
	customData.customCommandId = customCmdId;
	customData.paramCount = nParam;

	{
		void* params[] = { (void*)(size_t)args..., nullptr };
		memcpy_s(&customData.params[0], paramSize, &params[0], paramSize);
	}

	ReturnData& returnData = customData.returnData;
	int iRes;
	DWORD invokeResult;
	iRes = spyClient->sendCommandToRemoteThread((BaseCmdData*)&customData, true, &invokeResult);
	if (iRes != 0) {
		return iRes;
	}
	if (invokeResult != 0) {
		return invokeResult;
	}
	handler(returnData);
	return iRes;
}