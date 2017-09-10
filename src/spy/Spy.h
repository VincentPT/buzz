#pragma once
#ifdef SPY_EXPORTS
#define SPY_API __declspec(dllexport)
#else
#define SPY_API __declspec(dllimport)
#endif // SPY_EXPORTS