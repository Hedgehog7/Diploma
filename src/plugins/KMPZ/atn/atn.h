#ifndef ATN_H
#define ATN_H

#include <string>
#include <vector>
#include <regex>
#include <map>
#include <functional>

#include<QDir>

/* Класс, отвечающий за анализ темпоральных лексем
 * НЕ РАБОТАЕТ
 * пометка от 25.04.2020
*/

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
