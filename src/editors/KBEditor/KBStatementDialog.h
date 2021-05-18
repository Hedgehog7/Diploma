#pragma once
#include <QDialog>
#include "KBStatement.h"
#include "KBStatementWidget.h"

class KBStatementDialog : public QDialog
{
    Q_OBJECT
public:
    KBStatementDialog(KBStatement* statement, KnowledgeBase* kb, QWidget* parent = 0);
    KBStatement* m_statement;
    bool isValid();
private slots:
    void beforeClose();
    void checkValid();
    void reject();
private:
    KBStatement* m_backup;
    KBStatementWidget* statementWidget;
    KnowledgeBase* m_kb;
    QPushButton* okBtn;
    void buildUi();
};
