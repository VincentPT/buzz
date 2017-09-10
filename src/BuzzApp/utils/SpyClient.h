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
private:
	SpyClient();
public:
	static SpyClient* getInstance();
	static DWORD getProcessByName(const char* processName);
	static HMODULE getModuleByName(DWORD th32ProcessID, const char* moduleName, std::string& modulePath);
	static std::string getModuleName(const std::string& dllPath);

	bool startMonitorProcess(const char* processName, const std::string& rootDllPath, const std::list<std::string>& dependencyDllPaths);
	bool checkTargetAvaible();
	HMODULE injectDLL(const std::string& dllPath);

	bool executeRemoteCommand(void* remoteProc, const void* data, int dataSize, DWORD* executeResult, void** pptrRemote = nullptr);
	int sendCommandToRemoteThread(void* commandData, int commandSize, ReturnData* pReturnData = nullptr);
	int sendCommandToRemoteThread(BaseCmdData* commandData, ReturnData* pReturnData = nullptr);
};