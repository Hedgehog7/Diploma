#include "test.h"
#include <QtCore/QCoreApplication>

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	Test test;
	test.testOnATN();
	test.createDebugHtml();
	return a.exec();
}

