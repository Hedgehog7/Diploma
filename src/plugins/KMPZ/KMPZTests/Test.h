#ifndef TEST_H
#define TEST_H

#include "test.h"
#include "../atn/atn.h" 

#include <QString>

class Test
{
public:
	void testOnATN();
	void Test::createDebugHtml();
private:
	ATN _atn;
	const QString _debugPath = "../../../../bin/libTemp/debug.html";
};

#endif // TEST_H
