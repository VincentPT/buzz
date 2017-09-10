#include <Windows.h>
#include <stdio.h>

#if _DEBUG

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		printf_s("DLL_PROCESS_ATTACH\n");
	}
	else if (ul_reason_for_call == DLL_PROCESS_DETACH)
	{
		printf_s("DLL_PROCESS_DETACH\n");
	}
	else if (ul_reason_for_call == DLL_THREAD_ATTACH)
	{
		printf_s("DLL_THREAD_ATTACH\n");
	}
	else if (ul_reason_for_call == DLL_THREAD_DETACH)
	{
		printf_s("DLL_THREAD_DETACH\n");
	}
	return TRUE;
}

#endif