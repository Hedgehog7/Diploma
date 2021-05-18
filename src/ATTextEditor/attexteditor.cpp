#include "attexteditor.h"

APlugin * AT_CREATE_PLUGIN_FN()
{
	return new ATTextEditorPlugin();
}

ATTextDocument::ATTextDocument()
{
	setDocumentLayout(new QPlainTextDocumentLayout(this));
}

AError ATTextDocument::saveToFile(const std::string & file_name) 
{
	_Base::saveToFile(file_name);

	auto txt = toPlainText();

	QFile file(QString::fromStdString(file_name));
	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		return AError(AT_ERROR_FILE_NOT_FOUND);

	file.write(txt.toUtf8());

	return AError();
}

AError ATTextDocument::loadFromFile(const std::string & file_name)
{
	_Base::loadFromFile(file_name);

	QFile file(QString::fromStdString(file_name));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return AError(AT_ERROR_FILE_NOT_FOUND);

	QString txt;
	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		txt.append(line);
	//	txt.append("\n");
	}

	setPlainText(txt);

	return AError();
}

ATTextEditor::ATTextEditor(AGUIEditorPlugin * _plug, QWidget *parent)
	:AGUIEditorInstance(_plug, parent)
{
	edText = new QPlainTextEdit(this);
	setCentralWidget(edText);
}


ATTextEditor::~ATTextEditor()
{
	delete edText;
}

void ATTextEditor::showDocument()
{
	auto doc = dynamic_cast<ATTextDocument*>(this->document().get());
	edText->setDocument(doc);
}

AError ATTextEditorPlugin::init(QToolBar * tb, QMenu * menu)
{
	return AError();
}

void ATTextEditorPlugin::openFile(ADocument * file)
{

}