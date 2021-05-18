
#ifndef ATGUI_config_h
#define ATGUI_config_h

#include "../global_conf.h"

#ifdef AT_GUI_EXPORT
	#define AT_GUI_API AT_EXPORT
#elif defined(AT_GUI_IMPORT)
	#define AT_GUI_API AT_IMPORT
#else
	#define AT_GUI_API 
#endif

#endif
