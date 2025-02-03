#pragma once
#ifdef DYNAMIC_LINK
	#ifdef DLL_EXPORT
		#define PALE_API __declspec(dllexport)
		#else
		#define PALE_API __declspec(dllimport)
	#endif
#else
	#define PALE_API
#endif