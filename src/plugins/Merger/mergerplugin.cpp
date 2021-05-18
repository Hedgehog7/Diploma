#include "mergerplugin.h"
#include "mainwindow.h"
#include <ATGUI/ATaskExecutionWindow.h>
#include <ATProject/ADocumentRef.h>
#include <ATCore/check.h>

APlugin * AT_CREATE_PLUGIN_FN()
{
  return new MergerPlugin();
}

MergerPlugin::MergerPlugin() :AUtilityPlugin("merger", "Combined Method For knowledge Acquisition") {
}

int MergerPlugin::executeTask(ATaskExecutionWindow * wnd, const std::string&, const std::vector<ADocumentRef*>&, const std::vector<ADocumentRef*>&) {
  auto dialog = new MergerWindow(wnd);
  wnd->setCentralWidget(dialog);

  return 0;
}

const std::vector<std::string> MergerPlugin::getCommands() {
  std::vector<std::string> res;
  return res;
}

AError MergerPlugin::executeCommand(const std::string&, std::string&) {
  return AError();
}
