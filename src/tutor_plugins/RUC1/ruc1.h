#ifndef RUC1_H
#define RUC1_H

#include "RUC1_global.h"

#include <ATGUI/APlugin.h>

class ATaskExecutionWindow;
class RUC1_EXPORT RUC1 : public AUtilityPlugin
{
public:
    RUC1();
    ~RUC1();
    // APlugin interface
public:
    int executeTask(ATaskExecutionWindow *wnd, const std::string &cmd_id, const std::vector<ADocumentRef *> &input_docs, const std::vector<ADocumentRef *> &out_docs, const std::string &login);

    // AUtilityPlugin interface
public:
    const std::vector<std::string> getCommands();
    AError executeCommand(const std::string &script, std::string &answer);
};

#endif // RUC1_H
