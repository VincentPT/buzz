#include "SpyClient.h"
#include "ScopeAutoFunction.hpp"

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

SpyClient::SpyClient() : _hTargetProcess(nullptr), _dwProcessId(0), _spyRootRemote(nullptr) {
}

SpyClient::~SpyClient() {
	if (_hTargetProcess != nullptr) {
		CloseHandle(_hTargetProcess);
	}
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

bool SpyClient::startMonitorProcess(const char* processName, const std::string& rootDllPath, const std::list<std::string>& dependencyDllPaths) {
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

	HMODULE rootBaseModule = injectDLL(_rootSpyDllPath);

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
	_spyRootRemote = (char*)rootBaseModule + spyRootRelativeAddress;

	cout << "monitor process " << processName << " started!" << std::endl;
	return true;
}

bool SpyClient::stopMonitorProcess() {
	if (_hTargetProcess != NULL) {
		BOOL res = CloseHandle(_hTargetProcess);
		_hTargetProcess = NULL;
		return res != FALSE;
	}

	return false;
}

bool SpyClient::restartMonitorProcess() {
	return startMonitorProcess(_processName.c_str(), _rootSpyDllPath, _dependencyDllPaths);
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

bool SpyClient::executeRemoteCommand(void* remoteProc, const void* data, int dataSize, DWORD* executeResult, void** pptrRemote) {
	if (_hTargetProcess == nullptr) {
		cout << "Invalid process handle" << std::endl;
		return false;
	}
	HANDLE hThread;
	void*   pRemoteData = nullptr;
	SIZE_T numberOfBytes;

	// auto free remote buffer function
	ScopeAutoFunction<function<void()>> autoFreeRemoteBufer([&]() {
		if (pRemoteData != nullptr && pptrRemote != nullptr) {
			if (VirtualFreeEx(_hTargetProcess, pRemoteData, dataSize, MEM_RELEASE) == FALSE) {
				cout << "[executeRemoteCommand]error in deallocate memory " << pRemoteData << " in remote process, GLE=" << GetLastError() << std::endl;
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

	hThread = ::CreateRemoteThread(_hTargetProcess, NULL, 0,
		(LPTHREAD_START_ROUTINE)remoteProc,
		pRemoteData, 0, NULL);
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

HMODULE SpyClient::injectDLL(const std::string& dllPath) {
	string moduleName = getModuleName(dllPath);
	string modulePath;

	// first, check if the module is already load into remote process
	HMODULE hRemoteLib = getModuleByName(GetProcessId(_hTargetProcess), moduleName.c_str(), modulePath);
	if (hRemoteLib == nullptr) {

		HMODULE hKernel32 = ::GetModuleHandle("Kernel32");
		void* remoteLoadLibraryFuncAddress = ::GetProcAddress(hKernel32, "LoadLibraryA");
		DWORD moduleBase32 = 0;
		bool res = executeRemoteCommand(remoteLoadLibraryFuncAddress, dllPath.c_str(), (int)(dllPath.size() + 1), &moduleBase32);
		if (res == false) {
			cout << "inject dll '" << dllPath << "' failed" << std::endl;
			return nullptr;
		}
		hRemoteLib = getModuleByName(GetProcessId(_hTargetProcess), moduleName.c_str(), modulePath);
	}

	return hRemoteLib;
}

int SpyClient::sendCommandToRemoteThread(void* commandData, int commandSize, ReturnData* pReturnData, DWORD* executeResult) {
	void*   pRemoteData = nullptr;

	// auto free remote buffer function
	ScopeAutoFunction<function<void()>> autoFreeRemoteBufer([&]() {
		if (pRemoteData != nullptr) {
			if (VirtualFreeEx(_hTargetProcess, pRemoteData, commandSize, MEM_RELEASE) == FALSE) {
				cout << "[sendCommandToRemoteThread]error in deallocate memory " << pRemoteData << " in remote process, GLE=" << GetLastError() << std::endl;
			}
		}
	});

	bool res = executeRemoteCommand(_spyRootRemote, commandData, commandSize, executeResult, &pRemoteData);
	if (res == false) {
		cout << "execute spy root function failed, GLE=" << GetLastError() << std::endl;
		return -1;
	}

	if (pReturnData) {
		constexpr int returnDataOffset = offsetof(CustomCommandCmdData, returnData);
		void* returnDataRemote = (char*)pRemoteData + returnDataOffset;

		int iRes = ReadDataFromRemoteProcess(_hTargetProcess, returnDataRemote, pReturnData, sizeof(ReturnData));
		if (iRes != 0) {
			return iRes;
		}
	}

	return 0;
}

int SpyClient::sendCommandToRemoteThread(BaseCmdData* commandData, ReturnData* pReturnData, DWORD* executeResult) {
	return sendCommandToRemoteThread(commandData, commandData->commandSize, pReturnData, executeResult);
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

const std::string& SpyClient::getProcessName() {
	return _processName;
}