#ifndef __DATAMININGPLUGIN_H__
#define __DATAMININGPLUGIN_H__

#include <QtCore/qglobal.h>
#include <ATGUI/APlugin.h>

class Q_DECL_EXPORT DataMiningPlugin : public AUtilityPlugin
{
  public:
    DataMiningPlugin();
    ~DataMiningPlugin();

    virtual int executeTask(ATaskExecutionWindow * wnd, const std::string & cmd_id, const std::vector<ADocumentRef*> & input_docs, const std::vector<ADocumentRef*> &, const std::string& login) override;

    virtual const std::vector<std::string> getCommands();
    virtual AError executeCommand(const std::string & script, std::string & answer);
};

#endif
