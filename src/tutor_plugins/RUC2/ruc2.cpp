#include "ruc2.h"
#include "mainwidget.h"
#include <ATGUI/ATaskExecutionWindow.h>
#include <ATProject/ADocumentRef.h>
#include <ATCore/check.h>

#include <QLabel>
#include <QFile>

APlugin * AT_CREATE_PLUGIN_FN()
{    return new RUC2();
}

RUC2::RUC2()
    :AUtilityPlugin("ruc2", "test ruc2")
{

}

int RUC2::executeTask(ATaskExecutionWindow * wnd, const std::string&, const std::vector<ADocumentRef*>&, const std::vector<ADocumentRef*>& out_docs) {
  auto dialog = new MainWidget(wnd);
  wnd->setCentralWidget(dialog);
  //dialog->outFName = out_docs[0]->absolutePath().toStdString();

  return 0;
}

const std::vector<std::string> RUC2::getCommands() {
  std::vector<std::string> res;
  return res;
}

AError RUC2::executeCommand(const std::string&, std::string&) {
  return AError();
}

