#pragma once
enum class CommandId : unsigned short
{
	FREE_BUFFER,
	LOAD_CUSTOM_FUNCTIONS,
	CUSTOM_COMMAND,
};

enum class CustomCommandId : unsigned short
{
	OPENCV_READ_MAT_OBJECT = 0,
	OPENCV_READ_CVPOINT_OBJECT,
	OPENCV_READ_CVPOINT2F_OBJECT,
	OPENCV_READ_CVRECT_OBJECT,
	OPENCV_READ_CVCONTOUR,
	OPENCV_READ_CVCONTOURS,

	// this must be placed at the end
	CUSTOM_COMMAND_END,
};

#pragma pack(push, 1)
struct ReturnData {
	char* customData;
	int sizeOfCustomData;
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

struct LoadCustomFunctionsCmdData {
	int commandSize;
	CommandId commandId;
	ReturnData returnData;
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