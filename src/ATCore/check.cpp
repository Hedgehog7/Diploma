
#include "check.h"
#include <exception>
#include <iostream>
#include <string>
#include <stdexcept>

class check_exception : public std::runtime_error
{
	using _Base = std::runtime_error;
public:
	check_exception(const char * filename, unsigned line_num)
		:_Base("Check failed (" + std::string(filename) + ":" + std::to_string(line_num) + ")")
	{}
};

void perform_check_impl(const char * filename, unsigned line_num, bool val, const char * msg)
{
	if (!val)
	{
		std::cout << "CHECK FAILED:" << filename << ":" << line_num << " - " << msg << std::endl;
#ifndef _WIN32
		__builtin_trap();
#endif
		throw check_exception(filename, line_num);
	}
}