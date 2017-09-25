#pragma once
#include "spy_interfaces.h"
#include <functional>

template<class T>
class ScopeAutoFunction {
	T _fx;
public:
	
	ScopeAutoFunction(const T& fx) {
		_fx = fx;
	}
	
	ScopeAutoFunction(T&& fx) {
		_fx = fx;
	}

	~ScopeAutoFunction() {
		_fx();
	}
};

class SpyClient;

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
	int freeBufferRes = spyClient->freeCustomCommandResult(&returnData);
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