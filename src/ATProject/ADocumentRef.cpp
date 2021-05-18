
#include "ADocumentRef.h"
#include "AProject.h"
#include <QFileInfo>

ADocumentRef::ADocumentRef(AProject * _project)
	:project(_project)
{}

QString ADocumentRef::absolutePath() const
{
	return project->dirPath + "/" + relativePath;
}

QString ADocumentRef::extension() const
{
	QFileInfo fi(relativePath);
	return fi.suffix();
}

