#include "CustomCommand.h"
#include "Spy.h"
#include "CustomCommandInvoker.h"

#include "../spylib/OpenCVObjectsCommands.h"

#include <iostream>

// using namespaces
using namespace std;

// extern variables declaration
extern int _runner_count;

// global variables
void* _custom_funcs[(unsigned short)CustomCommandId::CUSTOM_COMMAND_END] = { nullptr };

// macro definition
#define SET_CUSTOM_FUNCTION(cmdId, func) _custom_funcs[(unsigned short)(cmdId)] = func;
#define GET_CUSTOM_FUNCTION(cmdId) _custom_funcs[(unsigned short)(cmdId)];

// load custom functions function
// all custom functions must be set here
void loadCustomFunctions() {
	if (_custom_funcs[0] != nullptr) {
		return;
	}
	SET_CUSTOM_FUNCTION(CustomCommandId::OPENCV_READ_MAT_OBJECT, readCVMatObject);
	SET_CUSTOM_FUNCTION(CustomCommandId::OPENCV_READ_CVPOINT_OBJECT, readCVPointObject);
	SET_CUSTOM_FUNCTION(CustomCommandId::OPENCV_READ_CVPOINT2F_OBJECT, readCVPointFObject);
	SET_CUSTOM_FUNCTION(CustomCommandId::OPENCV_READ_CVRECT_OBJECT, readCVRectObject);
	SET_CUSTOM_FUNCTION(CustomCommandId::OPENCV_READ_CVCONTOUR, readCVContour);
	SET_CUSTOM_FUNCTION(CustomCommandId::OPENCV_READ_CVCONTOURS, readCVContours);
}

// this function use to invoke the corresponding custom function with the custom data
int invokeCustomCommand(CustomCommandCmdData* commandData) {
	if (commandData->commandSize != sizeof(CustomCommandCmdData) + (commandData->paramCount - 1) * sizeof(void*)) {
		cout << "Invalid custom command:" << commandData->commandSize << std::endl;
		return -1;
	}

	// load custom function before call
	loadCustomFunctions();

	// get function pointer
	void* fx = GET_CUSTOM_FUNCTION(commandData->customCommandId);
	if (fx == nullptr) {
		cout << "custom command '" << (unsigned short)commandData->customCommandId << "' has been not set" << std::endl;
		return -1;
	}

	if (_runner_count <= commandData->paramCount) {
		cout << "custom command " << commandData->paramCount << " parameters has been not supported" << std::endl;
		return -1;
	}

	CustomCommandInvoker invoker(fx, commandData->paramCount);
	commandData->returnData = invoker.invoke(commandData->params);
	return 0;
}