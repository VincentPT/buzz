#pragma once
enum class CommandId : unsigned short
{
	FREE_BUFFER,
	LOAD_PREDEFINED_FUNCTIONS,
	LOAD_CUSTOM_FUNCTIONS,
	UNLOAD_MODULE,
	GET_CUSTOM_FUNCTION_PTR,
	GET_MODULE,
	GET_MODULE_PATH,
	CUSTOM_COMMAND,
};

typedef unsigned short CustomCommandId;
typedef int ModuleId;

#define	CUSTOM_COMMAND_END  0xFFFF
#define	INVALID_MODULE_ID  (0xFFFFFFFF)

#pragma pack(push, 1)
struct ReturnData {
	char* customData;
	int sizeOfCustomData;
	int returnCode; // user return code, defined by user
};

struct BaseCmdData {
	int commandSize;
	CommandId commandId;
};

struct FreeBufferCmdData {
	int commandSize;
	CommandId commandId;
	void* buffer;
	int bufferSize;
};

struct LoadPredefinedReturnData {
	void* hModule;
	ModuleId moduleId;
};

struct LoadPredefinedCmdData {
	int commandSize;
	CommandId commandId;
	ReturnData returnData;// return data, in customData field, contain a pointer of LoadPredefinedReturnData
	char dllName[1];
};

struct LoadCustomFunctionsReturnData {
	void* hModule;
	ModuleId moduleId;
	CustomCommandId cmdIds[1];
};

struct LoadCustomFunctionsCmdData {
	int commandSize;
	CommandId commandId;
	ReturnData returnData;		// return data, in customData field, leading by RemoteModuleInfo
								// then each two-byte after contain function id coressponding with each function name
								// use CustomFunctionsReturnData to populate customData field;

	char fNames[1];				// leading by dll name then function names. Seperated by zero character and terminate by zero
};

struct ModuleData {
	void* hModule; // module handle
	unsigned short commandCount; // custom command count
	CustomCommandId cmdIds[1]; // custom command ids
};

struct ModulePathData {
	void* hModule;
	int bufferSize;
	char path[1];
};


struct GetModulePathCmdData {
	int commandSize;
	CommandId commandId;
	ModuleId moduleId;
	ReturnData returnData; // return data, in customData is a pointer point to ModulePathData structure
};

struct GetModuleCmdData {
	int commandSize;
	CommandId commandId;
	ModuleId moduleId;
	ReturnData returnData; // return data, in customData is a pointer point to ModuleData structure
};

struct GetFunctionPtrCmdData {
	int commandSize;
	CommandId commandId;
	CustomCommandId customCommandId; //[input] custom command which associate with the function
	void* ptr; // [output]: function address.
};


struct UnloadModuleCmdData {
	int commandSize;
	CommandId commandId;
	int moduleId;
};


struct CustomCommandCmdData {
	int commandSize;
	CommandId commandId;
	CustomCommandId customCommandId;
	ReturnData returnData;
	int paramCount;
	void* params[1];
};

#pragma pack(pop)

#define EMPTY_RETURN_DATA() {nullptr, 0}
#define SPY_ROOT_DLL_NAME "spy-engine-1.0.dll"

/**
*   brief an function interface that will be executed in spy engine, when a spy app send command LOAD_PREDEFINED_FUNCTIONS to the engine
*
*   getPredefinedFunctionCount should return number predefined function that the user want to loaded to the engine
**/
typedef int(*FGetPredefinedFunctionCount)();

/**
*   brief an function interface that should be called in user's spy library, at the time of loadPredefinedFunctions is executed
*
*   params
*		context: user spy library should pass it back to spy engine when use fx to set a custom command id. this param is supplied by spy engine through loadPredefinedFunctions
*		cmdId  : predefined command id
*		address: address of the function, the function will be called when a spy client send a command id has value equal to (cmdId + cmdBase)
*                cmdBase is supplied by the spy engine through function loadPredefinedFunctions
*
*   setPredefinedFunction return zero incase the command loaded success, non zero if not
**/
typedef int(*FSetPredefinedFunction)(void* context, CustomCommandId cmdId, void* address);

/**
*   brief an function interface that will be called in spy engine, when a spy app send command LOAD_PREDEFINED_FUNCTIONS to the engine
*
*   params
*		context: context pass by spy engine, user spy library should pass it back to spy engine when use fx to set a custom command id
*		fx     : a function pointer in spy engine, uer spy library should use it to load a custom command to spy engine
*		cmdBase: a command id base for predefined commands id will build up, spy library may store this value if need,
*				note that, this value is also returned to spy client from LOAD_PREDEFINED_FUNCTIONS commands
*
*   loadPredefinedFunctions should return zero to the engine know that loaded function should be kept in the engine for using in future
*                           or nonzero to notify that the engine should discard the loadding result
**/
typedef int(*FLoadPredefinedFunctions)(void* context, FSetPredefinedFunction fx, CustomCommandId cmdBase);

#define MAKE_RESULT_OF_LOAD_PREDEFINED_FUNC(n, base) (((n) << 16) | ((base) & 0xFFFF))
#define GET_NUMBER_OF_LOAD_PREDEFINED_FUNC(res) ((res) >> 16)
#define GET_BASE_OF_LOAD_PREDEFINED_FUNC(res) ((res) & 0xFFFF)