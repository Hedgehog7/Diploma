#ifndef __MERGERPLUGIN_H__
#define __MERGERPLUGIN_H__

#include <QtCore/qglobal.h>
#include <ATGUI/APlugin.h>

class Q_DECL_EXPORT MergerPlugin : public AUtilityPlugin
{
  public:
    MergerPlugin();
    ~MergerPlugin();

    virtual int executeTask(ATaskExecutionWindow * wnd, const std::string & cmd_id, const std::vector<ADocumentRef*> & input_docs, const std::vector<ADocumentRef*> &) override;

    virtual const std::vector<std::string> getCommands();
    virtual AError executeCommand(const std::string & script, std::string & answer);
};

#endif
