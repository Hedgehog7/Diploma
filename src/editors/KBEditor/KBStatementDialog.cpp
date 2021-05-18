#include "KBStatementDialog.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

KBStatementDialog::KBStatementDialog(KBStatement *statement, KnowledgeBase *kb, QWidget *parent)
    :QDialog(parent),
     m_kb(kb),
     m_statement(statement),
     m_backup(new KBStatement),
     okBtn(new QPushButton("Ok")),
     statementWidget(new KBStatementWidget(m_statement, kb, this))
{
    qDebug() << "Enter KBStatementDialog constructor";
    *m_backup = *m_statement;
    buildUi();
}

bool KBStatementDialog::isValid()
{
    return statementWidget->isValid();
}

void KBStatementDialog::beforeClose()
{
    this->accept();
}

void KBStatementDialog::checkValid()
{
    okBtn->setEnabled(isValid());
}

void KBStatementDialog::reject() {
    qDebug() << "Rejected!!";
    *m_statement = *m_backup;
    QDialog::reject();
}

void KBStatementDialog::buildUi()
{
    auto layout = new QVBoxLayout;
    layout->addWidget(statementWidget);
    layout->addWidget(okBtn);
    this->setLayout(layout);
    okBtn->setEnabled(isValid());
    connect(okBtn, &QPushButton::clicked, this, &KBStatementDialog::beforeClose);
    connect(statementWidget, &KBStatementWidget::changed, this, &KBStatementDialog::checkValid);
}
