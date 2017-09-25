#pragma once
#include <Windows.h>
#include <string>
#include <list>
#include "spy_interfaces.h"

class SpyClient {
protected:
	HANDLE						_hTargetProcess;
	HMODULE						_spyRootRemoteModule;
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

	bool inject(const char* processName, const std::string& rootDllPath, const std::list<std::string>& dependencyDllPaths);
	bool uninject();
	bool reinject();
	bool checkTargetAvaible();
	HMODULE injectDLL(const std::string& dllPath);

	bool executeRemoteCommand(void* remoteProc, void* remoteData, DWORD* executeResult);
	bool executeRemoteCommand(void* remoteProc, const void* data, int dataSize, DWORD* executeResult, void** pptrRemote = nullptr);
	int sendCommandToRemoteThread(void* commandData, int commandSize, bool readComandBack = false, DWORD* executeResult = nullptr);
	int sendCommandToRemoteThread(BaseCmdData* commandData, bool readComandBack = false, DWORD* executeResult = nullptr);
	int readCustomCommandResult(ReturnData* pReturnData, void** ppCustomData);
	int freeCustomCommandResult(ReturnData* pReturnData);

	// load predefined function in a spy library file
	// return -1 if failed,
	// incase success, hi word word is number of loadded command, low word is command id base for the predefined command
	int loadPredefinedFunctions(const char* dllFile, HMODULE* phModule = nullptr);
	int loadDynamicFunctions(const char* dllFile, const char* functions[], int functionCount, std::list<CustomCommandId>& loadedCustomFunctions, HMODULE* phModule = nullptr);
	int unloadModule(HMODULE hModule);
};