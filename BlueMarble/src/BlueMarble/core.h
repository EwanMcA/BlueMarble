#pragma once

#ifdef BM_PLATFORM_WINDOWS
	#ifdef BM_BUILD_DLL
		#define BLUEMARBLE_API __declspec(dllexport)
	#else
		#define BLUEMARBLE_API __declspec(dllimport)
	#endif
#else
	#error BlueMarble currently only supports Windows.
#endif