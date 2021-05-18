
#ifndef ATCore_helpers_h
#define ATCore_helpers_h

#include "../config.h"
#include <string>
#include <functional>
#include <fstream>
//#include <libxml/tree.h>


AT_CORE_API void split_file_path(const std::string & path, std::string & dir, std::string & fname);

//LIBXML

AT_CORE_API void open_ofstream(std::ofstream & os, const std::string & path, std::ios_base::openmode mode = std::ios_base::out);
AT_CORE_API void open_ifstream(std::ifstream & os, const std::string & path, std::ios_base::openmode mode = std::ios_base::in);
AT_CORE_API std::wstring utf8_to_wstring(const std::string & utf8_str);
AT_CORE_API std::string wstring_to_utf8(const std::wstring & wstr);
AT_CORE_API bool read_file_content(const std::string & fname, std::string & data);


template<typename It>
struct print_range_t
{
	print_range_t(It _first, It _last, char _delimeter)
		:first(_first), last(_last), delimeter(_delimeter)
	{
		printFun = [](std::ostream & os, typename It::value_type val) {
			os << val;
		};
	}

	friend std::ostream & operator<<(std::ostream & os, const print_range_t & obj)
	{
		bool is_first = true;
		while (obj.first != obj.last)
		{
			if (is_first)
				is_first = false;
			else
				os << obj.delimeter;

			obj.printFun(os, *obj.first++);
		}
		return os;
	}

	mutable It first;
	It last;
	std::function<void(std::ostream &, typename It::value_type)> printFun;
	char delimeter;
};

template<typename It>
print_range_t<It> print_range(It first, It last, char delimeter = ',')
{
	return print_range_t<It>(first, last, delimeter);
}

template<typename It, typename F>
print_range_t<It> print_range(It first, It last, F fun)
{
	print_range_t<It> obj(first, last);
	obj.printFun = fun;
	return std::move(obj);
}


#endif
