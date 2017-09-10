#pragma once
#ifdef SPYLIB_EXPORTS
#define SPYLIB_API __declspec(dllexport)
#else
#define SPYLIB_API __declspec(dllimport)
#endif // SPYLIB_EXPORTS