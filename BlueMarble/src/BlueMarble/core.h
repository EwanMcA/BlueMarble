#pragma once

#include <memory>

#ifdef BM_PLATFORM_WINDOWS
	#ifdef BM_BUILD_DLL
		#define BLUEMARBLE_API __declspec(dllexport)
	#endif
#else
	#error BlueMarble currently only supports Windows.
#endif

#ifdef BM_DEBUG
	#define BM_ENABLE_ASSERTS
#endif

#ifdef BM_ENABLE_ASSERTS
	#define BM_ASSERT(x, ...) { if(!(x)) { BM_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define BM_CORE_ASSERT(x, ...) { if(!(x)) { BM_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define BM_ASSERT(x, ...)
	#define BM_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define BM_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace BlueMarble {

    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T>
    using Ref = std::shared_ptr<T>;

}