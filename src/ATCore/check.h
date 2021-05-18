
#ifndef U_core_check_h
#define U_core_check_h

#include "config.h"

#define assert_check_eq(a, b) perform_check(a == b)

#if 1
	#define perform_check(...) perform_check_impl(__FILE__, __LINE__, __VA_ARGS__)
#else
	#define perform_check(...) {}
#endif

AT_CORE_API void perform_check_impl(const char * filename, unsigned line_num, bool val, const char * msg = "");

#endif
