

#ifndef ATCore_APluginManager_h
#define ATCore_APluginManager_h

#include "config.h"
#include <ATCore/AError.h>
#include "APlugin.h"
#include <memory>
#include <vector>
#include <map>

#ifdef AT_PLATFORM_WIN
    #include <windows.h>
#endif

typedef APlugin * (*create_plugin_func)();

class AT_GUI_API ADynamicPlugin
{
public:
	ADynamicPlugin(const std::wstring & dll_path);
	~ADynamicPlugin();
	AError load();
	APlugin * plugin() const;
	const wchar_t * path() const;
private:
	std::wstring mLibraryPath;
    
#ifdef AT_PLATFORM_WIN
	HMODULE mLibInstance;
#else// defined(AT_PLATFORM_OSX)
    void * mLibInstance;
#endif
    
	create_plugin_func mFactoryFunction;
	APlugin * mPluginInstance;
};

class AT_GUI_API APluginManager
{
public:
	virtual ~APluginManager();
	AError loadPlugin(const std::string & plugin_path);
	APlugin * plugin(const std::string & plugin_id) const;
	//const std::vector<ADynamicPlugin*> & plugins(APlugin::Type type) const;

	/*
	Creates index for fast command-access.
	*/
	void buildCommandIndex();
    bool check_plugin(const std::string& plug);
	virtual AUtilityPlugin * getPluginForCommand(const std::string & cmd);
	AEditorPlugin * editorForExtension(const std::string & ext);
	std::map<std::string, APlugin*> plugins;
protected:
	std::vector<ADynamicPlugin*> mPlugins[static_cast<int>(APlugin::Type::Count)];
	std::map<std::string, AUtilityPlugin*> mCommandIndex;
};



#endif
