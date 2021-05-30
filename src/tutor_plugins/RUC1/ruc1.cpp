#include "ruc1.h"
#include "mainwidget.h"
#include <ATGUI/ATaskExecutionWindow.h>
#include <ATProject/ADocumentRef.h>
#include <ATCore/check.h>

#include <QLabel>
#include <QFile>

APlugin * AT_CREATE_PLUGIN_FN()
{    return new RUC1();
}

RUC1::RUC1()
    :AUtilityPlugin("ruc1", "test ruc1")
{

}

int RUC1::executeTask(ATaskExecutionWindow * wnd, const std::string&, const std::vector<ADocumentRef*>&, const std::vector<ADocumentRef*>& out_docs) {
  auto dialog = new MainWidget(wnd);
  wnd->setCentralWidget(dialog);
  //dialog->outFName = out_docs[0]->absolutePath().toStdString();

  return 0;
}

const std::vector<std::string> RUC1::getCommands() {
  std::vector<std::string> res;
  return res;
}

AError RUC1::executeCommand(const std::string&, std::string&) {
  return AError();
}

RUC1::~RUC1()
{
    delete this;
}
