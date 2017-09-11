#pragma once
#include "SpyClient.h"
#include "spylib_interfaces.h"

#include <functional>

class BuzzSpyClient : public SpyClient {
public:
	BuzzSpyClient();
	~BuzzSpyClient();

	bool startMonitorProcess(const char* processName);

	int readCVMat(void* address, const std::function<void(ImageRawData*&)>& handler);
};