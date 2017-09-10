#include "CustomCommandInvoker.h"

class Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) = 0;
};

class Runner0 : public Runner {
public:	
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)();
		return ((FX)fx)();
	}
};

class Runner1 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*);
		return ((FX)fx)(params[0]);
	}
};

class Runner2 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*, void*);
		return ((FX)fx)(params[0], params[1]);
	}
};

class Runner3 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*, void*, void*);
		return ((FX)fx)(params[0], params[1], params[2]);
	}
};

class Runner4 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*, void*, void*, void*);
		return ((FX)fx)(params[0], params[1], params[2], params[3]);
	}
};

class Runner5 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*, void*, void*, void*, void*);
		return ((FX)fx)(params[0], params[1], params[2], params[3], params[4]);
	}
};

class Runner6 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*, void*, void*, void*, void*, void*);
		return ((FX)fx)(params[0], params[1], params[2], params[3], params[4], params[5]);
	}
};

class Runner7 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*, void*, void*, void*, void*, void*, void*);
		return ((FX)fx)(params[0], params[1], params[2], params[3], params[4], params[5], params[6]);
	}
};

class Runner8 : public Runner {
public:
	virtual ReturnData operator()(void* fx, void** params) {
		typedef ReturnData(*FX)(void*, void*, void*, void*, void*, void*, void*, void*);
		return ((FX)fx)(params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]);
	}
};

Runner0 _runner0;
Runner1 _runner1;
Runner2 _runner2;
Runner3 _runner3;
Runner4 _runner4;
Runner5 _runner5;
Runner6 _runner6;
Runner7 _runner7;
Runner8 _runner8;

Runner* _runners[] = {
	&_runner0,
	&_runner1,
	&_runner2,
	&_runner3,
	&_runner4,
	&_runner5,
	&_runner6,
	&_runner7,
	&_runner8,
};

int _runner_count = sizeof(_runners) / sizeof(_runners[0]);

CustomCommandInvoker::CustomCommandInvoker(void* fx, int paramSize) : _fx(fx)
{
	_runner = _runners[paramSize];
}


CustomCommandInvoker::~CustomCommandInvoker()
{
}

ReturnData CustomCommandInvoker::invoke(void** params) {
	return (*_runner)(_fx, params);
}
