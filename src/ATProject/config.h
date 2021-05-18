
#ifndef ATProject_config_h
#define ATProject_config_h

#include "../global_conf.h"

#ifdef AT_PROJECT_EXPORT
	#define AT_PROJECT_API AT_EXPORT
#elif defined(AT_PROJECT_IMPORT)
	#define AT_PROJECT_API AT_IMPORT
#else
	#define AT_PROJECT_API 
#endif


#endif
