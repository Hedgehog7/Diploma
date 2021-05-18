#include "DSFEditor.h"
#include <ATCore/check.h>
#include <ATProject/ADocumentRef.h>
#include <ATGUI/ATaskExecutionWindow.h>
#include <QFile>
#include <QTextEdit>

APlugin * AT_CREATE_PLUGIN_FN()
{
	return new DSFEditorPlugin();
}

ADSFEditor::ADSFEditor(AGUIEditorPlugin * _plug, QWidget *parent)
	:AGUIEditorInstance(_plug, parent)
{
	ui.setupUi(this);
}

ADSFEditor::~ADSFEditor()
{
}

void ADSFEditor::showDocument()
{
}

AError DSFEditorPlugin::init(QToolBar * tb, QMenu * menu)
{
	return AError();
}

void DSFEditorPlugin::openFile(ADocument * file)
{
}

int DSFEditorPlugin::executeTask(ATaskExecutionWindow * wnd, const std::string & cmd_id, const std::vector<ADocumentRef*> & input_docs, const std::vector<ADocumentRef*> & out_docs, const std::string& login)
{
	auto te = new QTextEdit(wnd);
	wnd->setCentralWidget(te);

	for (auto doc : out_docs)
	{
		auto path = doc->absolutePath();
		QFile file(path);
		bool r = file.open(QIODevice::WriteOnly);
		perform_check(r);
	}

	return 0;
}
