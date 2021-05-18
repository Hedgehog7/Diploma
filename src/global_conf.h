
#ifndef U_global_conf_h
#define U_global_conf_h

#if ULTRA_STATIC
	#define AT_EXPORT 
	#define AT_IMPORT 
#else
	#ifdef _WIN32
		#define AT_EXPORT __declspec(dllexport)
		#define AT_IMPORT __declspec(dllimport)
	#elif defined(__APPLE__)
		#define AT_EXPORT __attribute__((visibility("default")))
		#define AT_IMPORT __attribute__ ((__visibility__("default")))
	#else
		#define AT_EXPORT __attribute__ ((visibility ("default")))
		#define AT_IMPORT __attribute__ ((visibility ("default")))
	#endif
#endif


#if !defined(EMSCRIPTEN) && (defined(_WIN32) || defined(WIN32) || defined(WIN64))
    #define AT_PLATFORM_WIN 1
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
        #define AT_PLATFORM_IOS 1
    #elif TARGET_OS_MAC
        #define AT_PLATFORM_OSX 1
    #endif
#elif defined(__ANDROID__)
    #define AT_PLATFORM_ANDROID 1
#elif defined(EMSCRIPTEN)
	#define AT_PLATFORM_JS 1
#elif defined(__CYGWIN__)
	#define AT_PLATFORM_CYGWIN 1
#elif defined(__GNUG__)
	#define AT_PLATFORM_GCC 1
#else
    #define AT_PLATFORM_UNSUPPORTED 1
	#error "Platform not supported"
#endif

#define ENABLE_CHECK 1

#endif
