
#ifndef ADocumentRef_AProject_h
#define ADocumentRef_AProject_h

#include "config.h"
#include <QString>

class AProject;

class AT_PROJECT_API ADocumentRef
{
public:
	ADocumentRef(AProject * _project);
	QString absolutePath() const;
	QString extension() const;

	AProject * project;
	QString relativePath;
};


#endif
