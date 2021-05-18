
#ifndef ATCore_config_h
#define ATCore_config_h

#include "../global_conf.h"

#ifdef AT_CORE_EXPORT
	#define AT_CORE_API AT_EXPORT
#else
	#define AT_CORE_API AT_IMPORT
#endif

#endif
