#ifndef PROTOCOLS_H
#define PROTOCOLS_H

#include <ATGUI/APlugin.h>

#include <QtCore/qglobal.h>
class ATaskExecutionWindow;

#include "knowledgeField.h"
#include <QList>
#include <memory>

#include <QStandardItemModel>;
#include <QStringListModel>

class Q_DECL_EXPORT ImitationModel : public AUtilityPlugin
{
public:
	ImitationModel();
	~ImitationModel();

	virtual const std::vector<std::string> getCommands();
	virtual AError executeCommand(const std::string & script, std::string & answer);
	virtual int executeTask(ATaskExecutionWindow * wnd, const std::string & cmd_id, const std::vector<ADocumentRef*> & input_docs, const std::vector<ADocumentRef*> & out_docs) override;

	static QString ru(const char * text);

	void qtRepresentation(QWidget * wnd, KnowledgeField knowledgeField);
	void makeHypertext(KnowledgeField knowledgeField);
	void htmlRepresentation(KnowledgeField knowledgeField, QString protocolName);
	//void htmlRepresentation(KnowledgeField knowledgeField);

	QStringList makeRules(KnowledgeField knowledgeField);
	
private:
	std::unique_ptr<QStandardItemModel> _model;
	std::unique_ptr<QStringListModel> _smodel;
	std::unique_ptr<QStandardItemModel> _tmodel;
};

#endif // PROTOCOLS_H
