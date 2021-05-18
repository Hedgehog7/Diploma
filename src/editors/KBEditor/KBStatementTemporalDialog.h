#pragma once
#include <QDialog>
#include "KBStatementTemporal.h"
#include "KBStatementTemporalWidget.h"


class KBStatementTemporalDialog : public QDialog
{
    Q_OBJECT
public:
    KBStatementTemporalDialog(KBStatementTemporal* statement, KnowledgeBase* kb, QWidget* parent = 0);
    KBStatementTemporal* m_statement;
    bool isValid();
private slots:
    void beforeClose();
    void checkValid();
    void reject();
private:
    KBStatementTemporal* m_backup;
    KBStatementTemporalWidget* statementWidget;
    KnowledgeBase* m_kb;
    QPushButton* okBtn;
    void buildUi();
};
