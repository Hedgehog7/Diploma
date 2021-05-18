#include "atn.h"

#include <QFile>
#include <iostream> 
#include <fstream>
#include <stdlib.h>

#include <iostream>

namespace ATN_ns {
    std::string get_path()
    {
        QDir dir;
        QString path = dir.currentPath();
        path.truncate(path.lastIndexOf('/'));
        path.truncate(path.lastIndexOf('/'));
        path.truncate(path.lastIndexOf('/'));
        return path.toStdString();
    }
}

ATN::ATN()
{
    _inPath = ATN_ns::get_path() + "/resources/libTemp/input.txt";
    _outPath = ATN_ns::get_path() + "/resources/libTemp/output.xml";
}

void ATN::init(bool isTest)
{
	if (!isTest)
	{
		std::ofstream ofs1;
		ofs1.open(_inPath, std::ofstream::out | std::ofstream::trunc);
		ofs1.close();

	}
	std::ofstream ofs2;
	ofs2.open(_outPath, std::ofstream::out | std::ofstream::trunc);
	ofs2.close();
}

void ATN::loadData(const std::string& answer)
{
	std::ofstream out;
	out.open(_inPath, std::ios::app);
	out << answer + "\n";
	out.close();
}

void ATN::execute()
{
    system("~/fullat/src/plugins/KMPZ/KMPZTests/start.bat");
}

//при переносе в другие места необходимо изменить пути в здесь, в start.bat и config.blabla


