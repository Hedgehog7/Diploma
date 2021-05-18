#include "test.h"
#include <QFile>
#include <QXmlStreamReader>

#include <iostream> 
#include <fstream>

#include <QRegExp>

void Test::testOnATN()
{
	_atn.init(true);
	_atn.execute();
}

void Test::createDebugHtml()
{
	QFile file(_debugPath);
	QString data = "";

	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		while (!file.atEnd()) {
			data += file.readLine();
		}

		int index = data.indexOf("<p>");
		data.remove(index, data.indexOf("<table") - index);

		index = data.indexOf("<table", data.indexOf("<table") + 1);
		data.remove(index, data.lastIndexOf("</body>") - index);

		data.remove(QRegExp("href="));
	}
	file.close();

	file.open(QIODevice::WriteOnly | QIODevice::Truncate);
	file.write(data.toUtf8());

	file.close();

	/*std::ofstream ofs1;
	ofs1.open(_debugPath.toStdString(), std::ofstream::out | std::ofstream::trunc);
	ofs1 << data.toStdString();
	ofs1.close();*/
}