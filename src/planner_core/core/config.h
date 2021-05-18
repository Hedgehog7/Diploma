
#ifndef UltraCore_config_h
#define UltraCore_config_h

#if defined(WIN32) || defined(WIN64)
    #ifdef ULTRA_CORE_EXPORT
    	#define ULTRA_CORE_API __declspec(dllexport)
    #elif defined(ULTRA_CORE_IMPORT)
    	#define ULTRA_CORE_API __declspec(dllimport)
    #else
    	#define ULTRA_CORE_API 
    #endif

    #define USE_INTRINSIC 1
#else
    #define ULTRA_CORE_API 

    #define USE_INTRINSIC 0
#endif

#define USE_HDD_STORAGE 1

#define USE_TBB 0

#if USE_TBB

//#include <tbb/concurrent_queue.h>
//#include <tbb/parallel_sort.h>
//#include <tbb/concurrent_vector.h>

	#define QUEUE_T tbb::concurrent_queue
	#define VECTOR_T tbb::concurrent_vector
#else
	#define QUEUE_T std::queue
	#define VECTOR_T std::vector
#endif

#define TRACE_SOLUTION 1

#endif
