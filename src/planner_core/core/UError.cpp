
#include "UError.h"

core_exception::core_exception(const char * msg)
#ifdef __APPLE__
	:std::exception()
#elif defined(__GNUG__)
	:std::exception()
#else
    :std::exception(msg)
#endif
{
}
