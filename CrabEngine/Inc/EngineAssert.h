#pragma once

#ifdef _DEBUG
#define ASSERT(in_cond, in_msg, ...)\
if (!(in_cond))\
{\
	crab::Log::Error(in_msg, __VA_ARGS__);\
	__debugbreak();\
}
#else
#define ASSERT(in_cond, in_msg, ...)\
(void*)0
#endif