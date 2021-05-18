#ifndef RUC2_H
#define RUC2_H

#include "RUC2_global.h"

#include <ATGUI/APlugin.h>

class ATaskExecutionWindow;
class RUC2_EXPORT RUC2 : public AUtilityPlugin
{
public:
    RUC2();
    ~RUC2();
    // APlugin interface
public:
    int executeTask(ATaskExecutionWindow *wnd, const std::string &cmd_id, const std::vector<ADocumentRef *> &input_docs, const std::vector<ADocumentRef *> &out_docs);

    // AUtilityPlugin interface
public:
    const std::vector<std::string> getCommands();
    AError executeCommand(const std::string &script, std::string &answer);
};

#endif // RUC2_H
