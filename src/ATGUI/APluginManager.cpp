
#include "APluginManager.h"
#include "APlugin.h"
#include <ATCore/utils/helpers.h>
#include <ATCore/check.h>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <string.h>

#ifdef AT_PLATFORM_WIN
    #include <windows.h>
#else// defined(AT_PLATFORM_OSX)
    #include <dlfcn.h>
#endif

using namespace std;


bool APluginManager::check_plugin(const std::string& plug){

    auto it = plugins.find(plug);
    if (it != plugins.end())
        return true;
    else
        return false;
}


ADynamicPlugin::ADynamicPlugin(const std::wstring & dll_path)
    :mLibraryPath(dll_path), mLibInstance(nullptr), mFactoryFunction(nullptr), mPluginInstance(nullptr)
{
}

AError ADynamicPlugin::load()
{
	auto lib_path = wstring_to_utf8(mLibraryPath);
#ifdef AT_PLATFORM_WIN
	mLibInstance = LoadLibrary(mLibraryPath.c_str());

	if(!mLibInstance)
		return AError(AT_ERROR_FILE_NOT_FOUND, "File not found");
	
	mFactoryFunction = create_plugin_func(GetProcAddress(mLibInstance, AT_CREATE_PLUGIN_FNNAME));

	if(!mFactoryFunction)
	{
		FreeLibrary(mLibInstance);
		return AError(AT_ERROR_INVALID_FILE, "Invalid file");
	}

#else// defined(AT_PLATFORM_OSX)
    //.so
    std::cout << "Trying to load " << lib_path << std::endl;
    mLibInstance = dlopen(lib_path.c_str(), RTLD_LAZY);
    if (mLibInstance == nullptr)
    {
        std::cout << dlerror() << std::endl;
        return AError(AT_ERROR_FILE_NOT_FOUND);
    }

    mFactoryFunction = (create_plugin_func)dlsym(mLibInstance, AT_CREATE_PLUGIN_FNNAME);
    perform_check(mFactoryFunction != nullptr);

#endif
    mPluginInstance = mFactoryFunction();
    
	return AError();
}


ADynamicPlugin::~ADynamicPlugin()
{
	if(mLibInstance)
	{
#ifdef AT_PLATFORM_WIN
        FreeLibrary(mLibInstance);
#else// defined(AT_PLATFORM_OSX)
        dlclose(mLibInstance);
#endif
	}
}

APlugin * ADynamicPlugin::plugin() const
{
	return mPluginInstance;
}

const wchar_t * ADynamicPlugin::path() const
{
	return mLibraryPath.c_str();
}

APluginManager::~APluginManager()
{
	for(int i = 0; i < static_cast<int>(APlugin::Type::Count); ++i)
	{
		for(auto plugin : mPlugins[i])
		{
			delete plugin;
		}

		mPlugins[i].clear();
	}
}

AError APluginManager::loadPlugin(const std::string & plugin_path)
{
	//__builtin_trap();

	/*wchar_t * buf = new wchar_t[plugin_path.size() ];
	size_t num_chars = mbstowcs(buf, plugin_path.c_str(), plugin_path.size() );
	wstring ws( buf, num_chars );
	delete[] buf;*/
    std::cout << "Trying to load from " << plugin_path << std::endl;
	auto path = utf8_to_wstring(plugin_path);

	ADynamicPlugin * new_plugin = new ADynamicPlugin(path);
	AError res = new_plugin->load();

	if (res.OK())
	{
		auto plugin_type = new_plugin->plugin()->type();
		mPlugins[static_cast<int>(plugin_type)].push_back(new_plugin);
		plugins.insert(std::make_pair(new_plugin->plugin()->name(), new_plugin->plugin()));
	}

	return res;
}
/*
const std::vector<ADynamicPlugin*> & APluginManager::plugins(APlugin::Type type) const
{
	return mPlugins[static_cast<int>(type)];
}*/

void APluginManager::buildCommandIndex()
{
	mCommandIndex.clear();

	for(auto dp : mPlugins[static_cast<int>(APlugin::Type::Utility)])
	{
		auto * plugin(static_cast<AUtilityPlugin*>(dp->plugin()));

		for(auto & cmd : plugin->getCommands())
		{
			auto cmd_it = mCommandIndex.find(cmd);
			if(cmd_it == mCommandIndex.end())
			{
				mCommandIndex.insert(pair<string, AUtilityPlugin*>(cmd, plugin));
			}
			else
				throw AException("Plugin '" + plugin->name() + "' invalid registration, command " + cmd + " already has executor!");
		}
	}
}

AUtilityPlugin * APluginManager::getPluginForCommand(const std::string & cmd)
{
	auto cmd_it = mCommandIndex.find(cmd);
	if(cmd_it == mCommandIndex.end())
		return nullptr;
	else
		return cmd_it->second;
}

AEditorPlugin * APluginManager::editorForExtension(const std::string & ext)
{
	auto & editors = mPlugins[static_cast<int>(APlugin::Type::Editor)];
	auto ed_it = std::find_if(editors.begin(), editors.end(), [=](const ADynamicPlugin * plug){
		return static_cast<AEditorPlugin*>(plug->plugin())->documentExtension() == ext;
	});

	if(ed_it != editors.end())
		return static_cast<AEditorPlugin*>((*ed_it)->plugin());
	else
	{
		ed_it = std::find_if(editors.begin(), editors.end(), [=](const ADynamicPlugin * plug) {
			return static_cast<AEditorPlugin*>(plug->plugin())->documentExtension() == "*";
		});
		if (ed_it != editors.end())
			return static_cast<AEditorPlugin*>((*ed_it)->plugin());
		else
			return nullptr;
	}
}

APlugin * APluginManager::plugin(const std::string & plugin_id) const
{
	auto it = plugins.find(plugin_id);
	if (it == plugins.end())
		return nullptr;
	else
		return it->second;
}
