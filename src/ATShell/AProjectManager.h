
#ifndef ATCore_AProjectManager_h
#define ATCore_AProjectManager_h

#include <ATCore/config.h>
#include <ATCore/AError.h>
#include <string>

class AProject;

class AProjectManager
{
public:
	AProjectManager();
	AProject * project() const;

	virtual int closeProject();
	AProject * createProject();
	AError saveProject() const;
	AProject * openProject(const std::string & path);
	void buildClean(AErrorMessenger & os) const;
	virtual void setCurrentProject(AProject * p_project);
//private:
	AProject * m_pProject;
	AErrorMessenger * atOut;
};

#endif
