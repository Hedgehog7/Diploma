#include "dataminingplugin.h"
#include "mainwindow.h"
#include <ATGUI/ATaskExecutionWindow.h>
#include <ATProject/ADocumentRef.h>
#include <ATCore/check.h>

APlugin * AT_CREATE_PLUGIN_FN()
{
  return new DataMiningPlugin();
}

DataMiningPlugin::DataMiningPlugin() :AUtilityPlugin("data_mining", "Combined Method For knowledge Acquisition") {
}

int DataMiningPlugin::executeTask(ATaskExecutionWindow * wnd, const std::string&, const std::vector<ADocumentRef*>&, const std::vector<ADocumentRef*>& out_docs, const std::string& login) {
  auto dialog = new DataMiningWindow(wnd);
  wnd->setCentralWidget(dialog);

  dialog->outFName = out_docs[0]->absolutePath().toStdString();

  return 0;
}

const std::vector<std::string> DataMiningPlugin::getCommands() {
  std::vector<std::string> res;
  return res;
}

AError DataMiningPlugin::executeCommand(const std::string&, std::string&) {
  return AError();
}
