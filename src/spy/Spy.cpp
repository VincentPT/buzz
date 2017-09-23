#include "Spy.h"
#include "CustomCommand.h"
#include <Windows.h>

#include <iostream>

using namespace std;

int freeBufferCommand(FreeBufferCmdData* commandData) {
	if (commandData->commandSize != sizeof(FreeBufferCmdData)) {
		cout << "Invalid free buffer command" << std::endl;
		return -1;
	}
	cout << "deallocated buffer:" << commandData->buffer << std::endl;
	free(commandData->buffer);
	//VirtualFree(commandData->buffer, commandData->bufferSize, MEM_RELEASE);

	return 0;
}

extern "C" {
	SPY_API DWORD spyRoot(BaseCmdData* param) {
		switch (param->commandId)
		{
		case CommandId::CUSTOM_COMMAND:
			return invokeCustomCommand((CustomCommandCmdData*)param);
		case CommandId::FREE_BUFFER:
			return freeBufferCommand((FreeBufferCmdData*)param);
		case CommandId::LOAD_CUSTOM_FUNCTIONS:
			break;
		default:
			break;
		}

		return 0;
	}
}