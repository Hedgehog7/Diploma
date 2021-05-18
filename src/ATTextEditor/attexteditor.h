#ifndef ATTEXTEDITOR_H
#define ATTEXTEDITOR_H

#include "attexteditor_global.h"
#include <ATCore/ADocument.h>
#include <ATGUI/AEditor.h>
#include <QtWidgets/QWidget>
#include <QTextDocument>
#include <QPlainTextEdit>

class ATTextDocument : public ADocument, public QTextDocument
{
	using _Base = ADocument;
public:
	ATTextDocument();
	virtual AError saveToFile(const std::string & file_name) override;
	virtual AError loadFromFile(const std::string & file_name) override;
};

class ATTEXTEDITOR_EXPORT ATTextEditor : public AGUIEditorInstance
{
public:
	ATTextEditor(AGUIEditorPlugin * _plug, QWidget *parent = 0);
	~ATTextEditor();
	virtual void showDocument() override;

private:
	QPlainTextEdit * edText;
};


class ATTextEditorPlugin : public AGUITEditorPlugin<ATTextDocument, ATTextEditor>
{
	using _Base = AGUITEditorPlugin<ATTextDocument, ATTextEditor>;
public:
	ATTextEditorPlugin()
		:_Base("*", "Plain Text", "Plain Text Editor", "text_editor", "Default plain text editor.")
	{}

	virtual AError init(QToolBar * tb, QMenu * menu);
	virtual void openFile(ADocument * file);
	//virtual int executeTask(ATaskExecutionWindow * wnd, const std::string & cmd_id, const std::vector<ADocumentRef*> & input_docs, const std::vector<ADocumentRef*> & out_docs) override;
};


#endif // ATTEXTEDITOR_H
