#include "KBStatementTemporalDialog.h"

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

KBStatementTemporalDialog::KBStatementTemporalDialog(KBStatementTemporal *statement, KnowledgeBase *kb, QWidget *parent)
    :QDialog(parent),
     m_kb(kb),
     m_statement(statement),
     m_backup(new KBStatementTemporal),
     okBtn(new QPushButton("Ok")),
     statementWidget(new KBStatementTemporalWidget(m_statement, kb, this))
{
    qDebug() << "Enter KBStatementTemporalDialog constructor";
    *m_backup = *m_statement;
    buildUi();
}

bool KBStatementTemporalDialog::isValid()
{
    return statementWidget->isValid();
}

void KBStatementTemporalDialog::beforeClose()
{
    this->accept();
}

void KBStatementTemporalDialog::checkValid()
{
    okBtn->setEnabled(isValid());
}

void KBStatementTemporalDialog::reject() {
    qDebug() << "Rejected!!";
    *m_statement = *m_backup;
    QDialog::reject();
}

void KBStatementTemporalDialog::buildUi()
{
    auto layout = new QVBoxLayout;
    layout->addWidget(statementWidget);
    layout->addWidget(okBtn);
    this->setLayout(layout);
    okBtn->setEnabled(isValid());
    connect(okBtn, &QPushButton::clicked, this, &KBStatementTemporalDialog::beforeClose);
    connect(statementWidget, &KBStatementTemporalWidget::changed, this, &KBStatementTemporalDialog::checkValid);
}
