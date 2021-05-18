#include "ruc1.h"
#include "mainwidget.h"
#include <ATGUI/ATaskExecutionWindow.h>
#include <ATProject/ADocumentRef.h>
#include <ATCore/check.h>

#include <QLabel>
#include <QFile>
#include <QDebug>

APlugin * AT_CREATE_PLUGIN_FN()
{    return new RUC1();
}

RUC1::RUC1()
    :AUtilityPlugin("ruc1", "test ruc1")
{

}

int RUC1::executeTask(ATaskExecutionWindow * wnd, const std::string&, const std::vector<ADocumentRef*>&, const std::vector<ADocumentRef*>& out_docs, const std::string& login) {
  auto dialog = new MainWidget(wnd);
  wnd->setCentralWidget(dialog);
  qDebug() << QString::fromStdString(login);
  if (out_docs.size() > 0){
    dialog->outFName.push_back(out_docs[0]->absolutePath().toStdString());
   }
  return 0;
}

const std::vector<std::string> RUC1::getCommands() {
  std::vector<std::string> res;
  return res;
}

AError RUC1::executeCommand(const std::string&, std::string&) {
  return AError();
}


