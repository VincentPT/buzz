#pragma once
#include "spy_interfaces.h"

class Runner;
class CustomCommandInvoker
{	
	Runner* _runner;
	void* _fx;
public:
	CustomCommandInvoker(void* fx, int paramSize);
	~CustomCommandInvoker();

	ReturnData invoke(void** params);
};

