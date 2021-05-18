
#ifndef ATProject_AProjectTemplate_h
#define ATProject_AProjectTemplate_h

#include "config.h"
#include <vector>
#include <QString>

class AT_PROJECT_API AProjectTemplate
{
public:
	AProjectTemplate();
	void load(const QString & fname);

	std::vector<QString> docs;

	QString name, dirPath;
};

#endif
