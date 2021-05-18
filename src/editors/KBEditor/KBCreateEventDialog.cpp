#include "KBCreateEventDialog.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

KBCreateEventDialog::KBCreateEventDialog(KnowledgeBase *kb, QWidget *parent)
    :QDialog(parent),
    nameEdit(new QLineEdit),
    formulaWidget(new FormulaWidget(kb, parent)),
    okBtn(new QPushButton("Ok")),
    m_event(new KBEvent)
{
    qDebug() << "Create Interval Dialog constructor";
    buildUi();
}

KBCreateEventDialog::KBCreateEventDialog(KBEvent *event, KnowledgeBase *kb, QWidget *parent)
    :QDialog(parent),
    nameEdit(new QLineEdit),
    formulaWidget(new FormulaWidget(&event->m_formula, kb, parent)),
    okBtn(new QPushButton("Ok")),
    m_event(event)
{
    qDebug() << "Create Interval Dialog constructor";
    buildUi();
}

bool KBCreateEventDialog::isValid()
{
    return m_event->m_name != "" && formulaWidget->isValid();
}

void KBCreateEventDialog::beforeClose()
{
    this->accept();
}

void KBCreateEventDialog::onNameChanged(const QString name)
{
    m_event->m_name = name;
    okBtn->setEnabled(isValid());
}

void KBCreateEventDialog::checkValid()
{
    okBtn->setEnabled(isValid());
}

void KBCreateEventDialog::buildUi()
{
    auto layout = new QVBoxLayout;
    auto nameLabel = new QLabel("Name");
    layout->addWidget(nameLabel);
    nameEdit->setText(m_event->m_name);
    layout->addWidget(nameEdit);

    auto formulaLabel = new QLabel("Open");
    layout->addWidget(formulaLabel);
    layout->addWidget(formulaWidget);

    layout->addWidget(okBtn);
    this->setLayout(layout);

    okBtn->setEnabled(isValid());

    connect(nameEdit, &QLineEdit::textChanged, this, &KBCreateEventDialog::onNameChanged);
    connect(okBtn, &QPushButton::clicked, this, &KBCreateEventDialog::beforeClose);
    connect(formulaWidget, &FormulaWidget::formulaChanged, this, &KBCreateEventDialog::checkValid);
}
