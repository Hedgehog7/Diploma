
#include "helpers.h"
#include <libxml/tree.h>
#include <codecvt>
#include <locale>
#include <cstring>

void split_file_path(const std::string & path, std::string & dir, std::string & fname)
{
	size_t found;
	found = path.find_last_of("/\\");
	dir = path.substr(0,found);
	fname  = path.substr(found+1);
}

template<typename T>
void open_fstream(T & stream, const std::string & path, std::ios_base::openmode mode)
{
#ifdef _WIN32
	std::wstring res = utf8_to_wstring(path);
	stream.open(res, mode);
#else
	stream.open(path.c_str(), mode);
#endif
}

void open_ofstream(std::ofstream & os, const std::string & path, std::ios_base::openmode mode)
{
	open_fstream(os, path, mode);
}

void open_ifstream(std::ifstream & is, const std::string & path, std::ios_base::openmode mode)
{
	open_fstream(is, path, mode);
}


bool read_file_content(const std::string & fname, std::string & data)
{
	std::ifstream is;// (fname, std::ifstream::binary);
	open_ifstream(is, fname, std::ifstream::binary);
	if (!is)
		return false;

	is.seekg(0, is.end);
	size_t length = is.tellg();
	is.seekg(0, is.beg);

	data.resize(length);
	is.read((char*)data.data(), length);

	return true;
}


std::wstring utf8_to_wstring(const std::string & utf8_str)
{
	#if _MSC_VER == 1900
		std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> convert;
		auto utf16_string = convert.from_bytes(utf8_str);
		return std::wstring(utf16_string.begin(), utf16_string.end());
	#else
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		std::u16string dest = convert.from_bytes(utf8_str);
		return std::wstring(dest.begin(), dest.end());
	#endif
}

std::string wstring_to_utf8(const std::wstring & wstr)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
	return convert.to_bytes(wstr.c_str());
}
