#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include "spy_interfaces.h"

class SpyClient {
protected:
	HANDLE						_hTargetProcess;
	DWORD						_dwProcessId;
	std::string					_processName;
	std::string					_rootSpyDllPath;
	std::list<std::string>		_dependencyDllPaths;
	void*						_spyRootRemote;
public:
	SpyClient();
	virtual ~SpyClient();
	const std::string& getProcessName();

	static DWORD getProcessByName(const char* processName);
	static HMODULE getModuleByName(DWORD th32ProcessID, const char* moduleName, std::string& modulePath);
	static std::string getModuleName(const std::string& dllPath);

	bool startMonitorProcess(const char* processName, const std::string& rootDllPath, const std::list<std::string>& dependencyDllPaths);
	bool stopMonitorProcess();
	bool restartMonitorProcess();
	bool checkTargetAvaible();
	HMODULE injectDLL(const std::string& dllPath);

	bool executeRemoteCommand(void* remoteProc, const void* data, int dataSize, DWORD* executeResult, void** pptrRemote = nullptr);
	int sendCommandToRemoteThread(void* commandData, int commandSize, ReturnData* pReturnData = nullptr, DWORD* executeResult = nullptr);
	int sendCommandToRemoteThread(BaseCmdData* commandData, ReturnData* pReturnData = nullptr, DWORD* executeResult = nullptr);
	int readCustomCommandResult(ReturnData* pReturnData, void** ppCustomData);
	int freeCustomCommandResult(ReturnData* pReturnData);

	int loadCustomDynamicFunction(const char* dllFile, const char* functions[], int functionCount, std::list<CustomCommandId>& loadedCustomFunctions, HMODULE* phModule = nullptr);
};