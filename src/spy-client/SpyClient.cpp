#include "SpyClient.h"
#include "SpyClientUtils.hpp"

#include <tlhelp32.h>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <conio.h>
#include <filesystem>

using namespace std;

int inline ReadDataFromRemoteProcess(HANDLE hProcess, void* remoteAddress, void* localBuffer, int size) {
	BOOL readWriteResult;
	SIZE_T numberOfBytes;

	readWriteResult = ::ReadProcessMemory(hProcess, remoteAddress, localBuffer, (SIZE_T) size, &numberOfBytes);
	if (readWriteResult == FALSE) {
		cout << "cannot read data to remote process, GLE=" << GetLastError() << std::endl;
		return -1;
	}
	if (numberOfBytes != size) {
		cout << "error in reading data to remote process, GLE=" << GetLastError() << std::endl;
		return -1;
	}

	return 0;
}

SpyClient::SpyClient() : _hTargetProcess(nullptr), _dwProcessId(0), _spyRootRemote(nullptr), _spyRootRemoteModule(nullptr) {
}

SpyClient::~SpyClient() {
	if (_hTargetProcess != nullptr) {
		CloseHandle(_hTargetProcess);
	}
}

const std::string& SpyClient::getProcessName() {
	return _processName;
}

DWORD SpyClient::getProcessByName(const char* processName) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			if (_stricmp(entry.szExeFile, processName) == 0)
			{
				return entry.th32ProcessID;
			}
		}
	}

	CloseHandle(snapshot);

	return 0;
}

HMODULE SpyClient::getModuleByName(DWORD th32ProcessID, const char* moduleName, std::string& modulePath) {
	MODULEENTRY32 entry;
	entry.dwSize = sizeof(entry);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, th32ProcessID);

	if (Module32First(snapshot, &entry) == TRUE)
	{
		while (Module32Next(snapshot, &entry) == TRUE)
		{			
			if (_stricmp(entry.szModule, moduleName) == 0)
			{
				modulePath = entry.szExePath;
				return entry.hModule;
			}
		}
	}

	CloseHandle(snapshot);
	return NULL;
}

std::string SpyClient::getModuleName(const std::string& dllPath) {
#if _WIN32
	using namespace std::experimental::filesystem::v1;	
	path thePath(dllPath);
	return thePath.filename().u8string();
#else
	auto it = dllPath.find_last_of('\\');
	auto it2 = dllPath.find_last_of('/');
	
	if (it == string::npos) {
		if (it2 == string::npos) {
			return dllPath;
		}
		it = it2;
	}
	else if (it2 != string::npos && it < it2) {
		it = it2;
	}
	return dllPath.substr(it + 1);
#endif //_WIN32
}

bool SpyClient::inject(const char* processName, const std::string& rootDllPath, const std::list<std::string>& dependencyDllPaths) {
	_processName = processName;

	_dwProcessId = getProcessByName(processName);
	if (_dwProcessId == 0) {
		printf_s("%s is not running\n", processName);
		return false;
	}

	//HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid);
	_hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _dwProcessId);
	if (_hTargetProcess == NULL) {
		printf_s("%s is not accessible\n", processName);
		return false;
	}

	_rootSpyDllPath = rootDllPath;
	_dependencyDllPaths = dependencyDllPaths;

	for (auto& path : _dependencyDllPaths) {
		if (injectDLL(path) == false) {
			printf_s("failed to inject %s to remote process\n", path.c_str());
			return false;
		}
	}

	_spyRootRemoteModule = injectDLL(_rootSpyDllPath);

	// compute root function address in remote process
	// base on module base and relative offset between module base
	// and addres of the function is current process
	HMODULE localHandle = LoadLibraryA(_rootSpyDllPath.c_str());
	if (localHandle == nullptr) {
		printf_s("%s cannot be loadded\n", _rootSpyDllPath.c_str());
		return false;
	}

	ScopeAutoFunction<std::function<void()>> autoFreeLib = [localHandle]() {
		// free the library in current process after the scope exit
		FreeLibrary(localHandle);
	};

	void* spyRootLocal = GetProcAddress(localHandle, "spyRoot");
	if (spyRootLocal == nullptr) {
		printf_s("%s is not valid\n", _rootSpyDllPath.c_str());
		return false;
	}

	// releative offset between module base and the function address
	size_t spyRootRelativeAddress = (size_t)spyRootLocal - (size_t)localHandle;

	// then, calculate function address in remote process
	_spyRootRemote = (char*)_spyRootRemoteModule + spyRootRelativeAddress;

	cout << "monitor process " << processName << " started!" << std::endl;
	return true;
}

bool SpyClient::uninject() {
	if (_hTargetProcess == NULL) {
		return false;
	}
	if (_spyRootRemoteModule != nullptr) {
		// uninject spy root dll
		HMODULE hKernel32 = ::GetModuleHandle("Kernel32");
		void* remoteFreeLibrary = ::GetProcAddress(hKernel32, "FreeLibrary");
		DWORD freeLiraryResult = 0;
		bool res = executeRemoteCommand(remoteFreeLibrary, _spyRootRemoteModule, &freeLiraryResult);
		if (res == false || freeLiraryResult == FALSE) {
			cout << "uninject root spy dll failed" << std::endl;
			return false;
		}

	}

	BOOL res = CloseHandle(_hTargetProcess);
	if (!res) {
		cout << "Close remote process handle failed" << std::endl;
	}
	_hTargetProcess = nullptr;
	_spyRootRemoteModule = nullptr;
	return res != FALSE;
}

bool SpyClient::reinject() {
	return inject(_processName.c_str(), _rootSpyDllPath, _dependencyDllPaths);
}

bool SpyClient::checkTargetAvaible() {
	using namespace std::experimental::filesystem::v1;

	if (_hTargetProcess == nullptr) {
		return false;
	}

	//DWORD exitCode;
	//// check if the process is still alive
	//if (GetExitCodeProcess(_hTargetProcess, &exitCode) != STILL_ACTIVE) {
	//	return false;
	//}

	// the process handle is now available but may be it is not the target process
	if (GetProcessId(_hTargetProcess) != _dwProcessId) {
		return false;
	}

	// check if the root spy dll is loaded to the target process
	auto moduleName = getModuleName(_rootSpyDllPath);
	string modulePath;
	auto hModule = getModuleByName(_dwProcessId, moduleName.c_str(), modulePath);
	if (hModule == NULL) {
		return false;
	}

	// check if the module path is same as expected to prevent using different spy dll version
	path expectedPath(_rootSpyDllPath);
	path actualPath(modulePath);

	return expectedPath == actualPath;
}

bool SpyClient::executeRemoteCommand(void* remoteProc, void* remoteData, DWORD* executeResult) {
	HANDLE hThread = ::CreateRemoteThread(_hTargetProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)remoteProc,
		remoteData, 0, NULL);
	if (hThread == NULL) {
		cout << "cannot execute function in remote process, GLE=" << GetLastError() << std::endl;
		return false;
	}

	::WaitForSingleObject(hThread, INFINITE);

	if (executeResult) {
		::GetExitCodeThread(hThread, executeResult);
	}

	// Clean up
	::CloseHandle(hThread);

	return true;
}

bool SpyClient::executeRemoteCommand(void* remoteProc, const void* data, int dataSize, DWORD* executeResult, void** pptrRemote) {
	//cout << __FUNCTION__ << std::endl;
	if (_hTargetProcess == nullptr) {
		cout << "Invalid process handle" << std::endl;
		return false;
	}

	void*   pRemoteData = nullptr;
	SIZE_T numberOfBytes;

	// auto free remote buffer function
	ScopeAutoFunction<function<void()>> autoFreeRemoteBufer([&]() {
		if (pRemoteData != nullptr && pptrRemote == nullptr) {
			if (VirtualFreeEx(_hTargetProcess, pRemoteData, 0, MEM_RELEASE) == FALSE) {
				cout << "error in deallocate memory " << pRemoteData << " in remote process, GLE=" << GetLastError() << std::endl;
			}
		}
	});

	pRemoteData = ::VirtualAllocEx(_hTargetProcess, NULL, (SIZE_T)dataSize,
		MEM_COMMIT, PAGE_READWRITE);
	if (pRemoteData == NULL) {
		cout << "cannot allocate memory in remote process, GLE=" << GetLastError() << std::endl;
		return false;
	}

	if (pptrRemote != nullptr) {
		*pptrRemote = pRemoteData;
	}

	auto readWriteResult = ::WriteProcessMemory(_hTargetProcess, pRemoteData, (void*)data,
		dataSize, &numberOfBytes);
	if (readWriteResult == FALSE) {
		cout << "cannot write data to remote process, GLE=" << GetLastError() << std::endl;
		return false;
	}
	if (numberOfBytes != dataSize) {
		cout << "error in writing data to remote process, GLE=" << GetLastError() << std::endl;
		return false;
	}

	return executeRemoteCommand(remoteProc, pRemoteData, executeResult);
}

HMODULE SpyClient::injectDLL(const std::string& dllPath) {
	string moduleName = getModuleName(dllPath);
	string modulePath;

	HMODULE hKernel32 = ::GetModuleHandle("Kernel32");
	void* remoteLoadLibraryFuncAddress = ::GetProcAddress(hKernel32, "LoadLibraryA");
	DWORD moduleBase32 = 0;
	bool res = executeRemoteCommand(remoteLoadLibraryFuncAddress, dllPath.c_str(), (int)(dllPath.size() + 1), &moduleBase32);
	if (res == false) {
		cout << "inject dll '" << dllPath << "' failed" << std::endl;
		return nullptr;
	}
#ifdef  _WIN64
	auto hRemoteLib = getModuleByName(GetProcessId(_hTargetProcess), moduleName.c_str(), modulePath);
#else
	auto hRemoteLib = (HMODULE)moduleBase32;
#endif //  _WIN64
	return hRemoteLib;
}

int SpyClient::sendCommandToRemoteThread(void* commandData, int commandSize, bool readComandBack, DWORD* executeResult) {
	//cout << __FUNCTION__ << std::endl;
	void*   pRemoteData = nullptr;

	// auto free remote buffer function
	ScopeAutoFunction<function<void()>> autoFreeRemoteBufer([&]() {
		if (pRemoteData != nullptr) {
			if (VirtualFreeEx(_hTargetProcess, pRemoteData, 0, MEM_RELEASE) == FALSE) {
				cout << "error in deallocate memory " << pRemoteData << " in remote process, GLE=" << GetLastError() << std::endl;
			}
		}
	});

	bool res = executeRemoteCommand(_spyRootRemote, commandData, commandSize, executeResult, &pRemoteData);
	if (res == false) {
		cout << "execute spy root function failed, GLE=" << GetLastError() << std::endl;
		return -1;
	}

	if (readComandBack) {
		int iRes = ReadDataFromRemoteProcess(_hTargetProcess, pRemoteData, commandData, commandSize);
		if (iRes != 0) {
			return iRes;
		}
	}

	return 0;
}

int SpyClient::sendCommandToRemoteThread(BaseCmdData* commandData, bool readComandBack, DWORD* executeResult) {
	return sendCommandToRemoteThread(commandData, commandData->commandSize, readComandBack, executeResult);
}

int SpyClient::readCustomCommandResult(ReturnData* pReturnData, void** ppCustomData) {
	if (_hTargetProcess == nullptr) {
		cout << "Invalid process handle" << std::endl;
		return -1;
	}

	if (pReturnData == nullptr) {
		cout << "null result pointer" << std::endl;
		return -1;
	}

	if (ppCustomData == nullptr) {
		cout << "null buffer pointer" << std::endl;
		return -1;
	}

	void* localCustomData = malloc(pReturnData->sizeOfCustomData);
	int iRes = ReadDataFromRemoteProcess(_hTargetProcess, pReturnData->customData, localCustomData, pReturnData->sizeOfCustomData);
	if (iRes != 0) {
		free(localCustomData);
		*ppCustomData = nullptr;
		return -1;
	}
	*ppCustomData = localCustomData;
	return 0;
}

int SpyClient::freeCustomCommandResult(ReturnData* pReturnData) {
	FreeBufferCmdData cmdData;
	cmdData.commandSize = sizeof(FreeBufferCmdData);
	cmdData.commandId = CommandId::FREE_BUFFER;
	cmdData.bufferSize = pReturnData->sizeOfCustomData;
	cmdData.buffer = pReturnData->customData;

	return sendCommandToRemoteThread((BaseCmdData*)&cmdData);
}

int SpyClient::loadPredefinedFunctions(const char* dllFile, ModuleId* pModuleId) {
	size_t size = strlen(dllFile) + 1;
	vector<char> rawParam(size + (sizeof(LoadPredefinedCmdData) - sizeof(LoadPredefinedCmdData::dllName)));

	LoadPredefinedCmdData& loadPredefinedFunctionCmdData = *(LoadPredefinedCmdData*)rawParam.data();
	loadPredefinedFunctionCmdData.commandId = CommandId::LOAD_PREDEFINED_FUNCTIONS;
	loadPredefinedFunctionCmdData.commandSize = (int)rawParam.size();
	auto& returnData = loadPredefinedFunctionCmdData.returnData;

	// fill dll file and function names to buffer
	char* buffer = loadPredefinedFunctionCmdData.dllName;
	memcpy_s(buffer, size, dllFile, size);

	// call load predefined functions in remote thread and retreive return value from remote function
	DWORD functionReturnVal;
	int iRes = sendCommandToRemoteThread((BaseCmdData*)&loadPredefinedFunctionCmdData, true, &functionReturnVal);
	if (iRes != 0) {
		return iRes;
	}
	// check the return data size
	if (returnData.sizeOfCustomData != sizeof(LoadPredefinedReturnData)) {
		cout << "return data by load predefined function is not correct" << std::endl;
		return -1;
	}

	LoadPredefinedReturnData* pLoadPredefinedReturnData;
	// read return buffer from remote process
	iRes = readCustomCommandResult(&returnData, (void**)&pLoadPredefinedReturnData);
	if (iRes != 0) {
		return iRes;
	}

	// fill the return data to local output data
	if (pModuleId) {
		*pModuleId = pLoadPredefinedReturnData->moduleId;
	}

	free(pLoadPredefinedReturnData);
	int freeBufferRes = freeCustomCommandResult(&returnData);

	return (int)functionReturnVal;
}

int SpyClient::loadDynamicFunctions(const char* dllFile, const char* functions[], int functionCount, list<CustomCommandId>& loadedCustomFunctions, ModuleId* pModuleId) {
	size_t totalSize = 0;
	size_t size;

	// compute size of buffer to store dll file and function names
	totalSize = strlen(dllFile) + 1;
	for (int i = 0; i < functionCount; i++) {
		totalSize += strlen(functions[i]) + 1;
	}

	// prepare command buffer
	vector<char> rawParam(totalSize + (sizeof(LoadCustomFunctionsCmdData) - sizeof(LoadCustomFunctionsCmdData::fNames)));
	LoadCustomFunctionsCmdData& loadCustomFunctionCmdData = *(LoadCustomFunctionsCmdData*)rawParam.data();
	loadCustomFunctionCmdData.commandId = CommandId::LOAD_CUSTOM_FUNCTIONS;
	loadCustomFunctionCmdData.commandSize = (int)rawParam.size();
	auto& returnData = loadCustomFunctionCmdData.returnData;

	// fill dll file and function names to buffer
	char* buffer = loadCustomFunctionCmdData.fNames;
	size = strlen(dllFile) + 1;
	memcpy_s(buffer, size, dllFile, size);
	buffer += size;
	
	for (int i = 0; i < functionCount; i++) {
		size = strlen(functions[i]) + 1;
		memcpy_s(buffer, size, functions[i], size);
		buffer += size;
	}

	// call load custom functions in remote thread
	int iRes = sendCommandToRemoteThread((BaseCmdData*)&loadCustomFunctionCmdData, true);
	if (returnData.sizeOfCustomData != sizeof(LoadCustomFunctionsReturnData) - sizeof(LoadCustomFunctionsReturnData::cmdIds) + functionCount * sizeof(CustomCommandId)) {
		cout << "return data by load custom function is not correct" << std::endl;
		return -1;
	if (iRes != 0) {
		return iRes;
	}
	// check the return data size
	}

	LoadCustomFunctionsReturnData* rawData;
	// read return buffer from remote process
	iRes = readCustomCommandResult(&returnData, (void**)&rawData);
	if (iRes != 0) {
		return iRes;
	}

	// fill the return data to local output data
	if (pModuleId) {
		*pModuleId = rawData->moduleId;
	}
	CustomCommandId* pCmdId = rawData->cmdIds;

	for (int i = 0; i < functionCount; i++) {
		loadedCustomFunctions.push_back(*pCmdId++);
	}

	free(rawData);

	if (freeCustomCommandResult(&returnData) != 0) {
		cout << "an error occurs when release custom data" << endl;
	}
	return iRes;
}

int SpyClient::unloadModule(ModuleId moduleId) {
	UnloadModuleCmdData cmdData;
	cmdData.commandId = CommandId::UNLOAD_MODULE;
	cmdData.commandSize = sizeof(UnloadModuleCmdData);
	cmdData.moduleId = moduleId;

	// call load predefined functions in remote thread and retreive return value from remote function
	DWORD functionReturnVal;
	int iRes = sendCommandToRemoteThread((BaseCmdData*)&cmdData, false, &functionReturnVal);
	if (iRes != 0) {
		return iRes;
	}
	return (int)functionReturnVal;
}

int SpyClient::getFunctionPtr(CustomCommandId cmdId, void** pptr) {
	GetFunctionPtrCmdData cmdData;
	cmdData.commandSize = sizeof(cmdData);
	cmdData.commandId = CommandId::GET_CUSTOM_FUNCTION_PTR;
	cmdData.customCommandId = cmdId;
	cmdData.ptr = nullptr;

	if (pptr) {
		*pptr = nullptr;
	}

	DWORD functionReturnVal;
	int iRes = sendCommandToRemoteThread((BaseCmdData*)&cmdData, true, &functionReturnVal);
	if (iRes != 0) {
		return iRes;
	}

	if (pptr) {
		*pptr = cmdData.ptr;
	}

	return (int)functionReturnVal;
}

int SpyClient::getModuleData(ModuleId moduleId, std::list<CustomCommandId>& loadedCustomFunctions, HMODULE* hModule) {
	GetModuleCmdData cmdData;
	cmdData.commandSize = sizeof(cmdData);
	cmdData.commandId = CommandId::GET_MODULE;
	cmdData.moduleId = moduleId;
	auto& returnData = cmdData.returnData;

	if (hModule) {
		*hModule = nullptr;
	}

	DWORD functionReturnVal;
	int iRes = sendCommandToRemoteThread((BaseCmdData*)&cmdData, true, &functionReturnVal);
	if (iRes != 0) {
		return iRes;
	}

	if (returnData.customData == nullptr || returnData.sizeOfCustomData == 0) {
		return -1;
	}

	ModuleData* rawData;
	iRes = readCustomCommandResult(&returnData, (void**)&rawData);
	if (iRes != 0) {
		return iRes;
	}

	// fill the return data to local output data
	if (hModule) {
		*hModule = (HMODULE)rawData->hModule;
	}
	CustomCommandId* pCmdId = rawData->cmdIds;
	auto functionCount = rawData->commandCount;

	for (int i = 0; i < functionCount; i++) {
		loadedCustomFunctions.push_back(*pCmdId++);
	}

	free(rawData);

	if (freeCustomCommandResult(&returnData) != 0) {
		cout << "an error occurs when release custom data" << endl;
	}

	return (int)functionReturnVal;
}

int SpyClient::getModulePath(ModuleId moduleId, std::string& path, HMODULE* hModule) {
	GetModulePathCmdData cmdData;
	cmdData.commandSize = sizeof(cmdData);
	cmdData.commandId = CommandId::GET_MODULE_PATH;
	cmdData.moduleId = moduleId;
	auto& returnData = cmdData.returnData;

	if (hModule) {
		*hModule = nullptr;
	}

	DWORD functionReturnVal;
	int iRes = sendCommandToRemoteThread((BaseCmdData*)&cmdData, true, &functionReturnVal);
	if (iRes != 0) {
		return iRes;
	}

	if (returnData.customData == nullptr || returnData.sizeOfCustomData == 0) {
		return -1;
	}

	ModulePathData* rawData;
	iRes = readCustomCommandResult(&returnData, (void**)&rawData);
	if (iRes != 0) {
		return iRes;
	}

	// fill the return data to local output data
	if (hModule) {
		*hModule = (HMODULE)rawData->hModule;
	}
	
	path = rawData->path;

	free(rawData);
	if (freeCustomCommandResult(&returnData) != 0) {
		cout << "an error occurs when release custom data" << endl;
	}
	return (int)functionReturnVal;
}