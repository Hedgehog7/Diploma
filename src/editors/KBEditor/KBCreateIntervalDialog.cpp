#include "KBCreateIntervalDialog.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>

KBCreateIntervalDialog::KBCreateIntervalDialog(KnowledgeBase *kb, QWidget* parent)
    :QDialog(parent),
    nameEdit(new QLineEdit),
    openWidget(new FormulaWidget(kb, parent)),
    closeWidget(new FormulaWidget(kb, parent)),
    okBtn(new QPushButton("Ok")),
    m_interval(new KBInterval)
{
    qDebug() << "Create Interval Dialog constructor";
    buildUi();
}

KBCreateIntervalDialog::KBCreateIntervalDialog(KBInterval* interval, KnowledgeBase *kb, QWidget* parent)
    :QDialog(parent),
    nameEdit(new QLineEdit),
    openWidget(new FormulaWidget(&interval->m_open, kb, parent)),
    closeWidget(new FormulaWidget(&interval->m_close, kb, parent)),
    okBtn(new QPushButton("Ok")),
    m_interval(interval)
{
    qDebug() << "Create Interval Dialog constructor";
    buildUi();
}

bool KBCreateIntervalDialog::isValid()
{
    return m_interval->m_name != "" && openWidget->isValid() && closeWidget->isValid();
}

void KBCreateIntervalDialog::beforeClose()
{
    this->accept();
}

void KBCreateIntervalDialog::onNameChanged(const QString name)
{
    m_interval->m_name = name;
    okBtn->setEnabled(isValid());

}

void KBCreateIntervalDialog::checkValid()
{
    okBtn->setEnabled(isValid());
}

void KBCreateIntervalDialog::buildUi()
{
    auto layout = new QVBoxLayout;
    auto nameLabel = new QLabel("Name");
    layout->addWidget(nameLabel);
    nameEdit->setText(m_interval->m_name);
    layout->addWidget(nameEdit);

    auto formulaLayout = new QHBoxLayout;

    auto openLayout = new QVBoxLayout;
    auto openLabel = new QLabel("Open");
    openLayout->addWidget(openLabel);
    openLayout->addWidget(openWidget);

    formulaLayout->addLayout(openLayout);

    auto closeLayout = new QVBoxLayout;
    auto closeLabel = new QLabel("Close");
    closeLayout->addWidget(closeLabel);
    closeLayout->addWidget(closeWidget);

    formulaLayout->addLayout(closeLayout);
    layout->addLayout(formulaLayout);

    layout->addWidget(okBtn);
    this->setLayout(layout);

    okBtn->setEnabled(isValid());

    connect(nameEdit, &QLineEdit::textChanged, this, &KBCreateIntervalDialog::onNameChanged);
    connect(okBtn, &QPushButton::clicked, this, &KBCreateIntervalDialog::beforeClose);
    connect(openWidget, &FormulaWidget::formulaChanged, this, &KBCreateIntervalDialog::checkValid);
    connect(closeWidget, &FormulaWidget::formulaChanged, this, &KBCreateIntervalDialog::checkValid);
}

