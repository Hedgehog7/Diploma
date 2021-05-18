#ifndef ATN_H
#define ATN_H

#include <string>
#include <vector>
#include <regex>
#include <map>
#include <functional>

#include<QDir>

class ATN
{
public:
    ATN();
	void init(bool isTest);
	void loadData(const std::string & answer);
	void execute();

private:
    std::string _inPath;
    std::string _outPath;
};

#endif // ATN_H
