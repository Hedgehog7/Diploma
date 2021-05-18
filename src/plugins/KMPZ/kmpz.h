#ifndef KMPZ_H
#define KMPZ_H

#include "kmpz_global.h"
#include "YOSDIInterpreter/YOSDIInterpreter.h"

#include <ATGUI/APlugin.h>

class ATaskExecutionWindow;

class KMPZ_EXPORT ATKMPZPlugin : public AUtilityPlugin
{
public:
	ATKMPZPlugin();
	~ATKMPZPlugin();

    virtual const std::vector<std::string> getCommands() override;
    virtual AError executeCommand(const std::string& script, std::string& answer) override;
    virtual int executeTask(ATaskExecutionWindow* wnd, const std::string& cmd_id, const std::vector<ADocumentRef*>& input_docs, const std::vector<ADocumentRef*>& out_docs, const std::string& login) override;

private:
    std::unique_ptr<YOSDIInterpreter> _interpreter;
};

#endif // KMPZ_H
