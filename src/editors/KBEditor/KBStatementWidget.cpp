#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include "KBStatementWidget.h"
KBStatementWidget::KBStatementWidget(KBStatement *statement, KnowledgeBase *kb, QWidget *parent)
    :KBElementWidget(parent),
     m_kb(kb),
     m_statement(statement),
     objectCombo(new QComboBox),
     attributeCombo(new QComboBox),
     valueEdit(new QLineEdit),
     beliefSpin(new QSpinBox),
     probabiltySpin(new QSpinBox),
     accuracySpin(new QSpinBox)
{
    qDebug() << "Enter KBStatementDialog constructor";
    buildUi();
}

bool KBStatementWidget::isValid()
{
    return m_statement->m_objectName != "" && m_statement->m_attributeName != "" && m_statement->m_value != "";
}

void KBStatementWidget::buildUi()
{
    auto mainLayout = new QVBoxLayout;
    auto statementLayout = new QHBoxLayout;
    statementLayout->addWidget(objectCombo);
    auto dotLabel = new QLabel(".");
    statementLayout->addWidget(dotLabel);
    statementLayout->addWidget(attributeCombo);
    auto assignLabel = new QLabel("=");
    statementLayout->addWidget(assignLabel);
    statementLayout->addWidget(valueEdit);
    mainLayout->addLayout(statementLayout);

    auto withLabel = new QLabel("WITH");
    mainLayout->addWidget(withLabel);

    auto formLayout = new QFormLayout;
    auto beliefLabel = new QLabel("Belief");
    formLayout->addRow(beliefLabel, beliefSpin);
    auto probabiltyLabel = new QLabel("Probabilty");
    formLayout->addRow(probabiltyLabel, probabiltySpin);
    auto accuracyLabel = new QLabel("Accuracy");
    formLayout->addRow(accuracyLabel, accuracySpin);

    mainLayout->addLayout(formLayout);
    this->setLayout(mainLayout);
    fillObjectsCombo();
    fillAttributeCombo();

    valueEdit->setText(m_statement->m_value);
    beliefSpin->setValue(m_statement->m_belief);
    probabiltySpin->setValue(m_statement->m_probabilty);
    accuracySpin->setValue(m_statement->m_accuracy);

    connect(valueEdit, &QLineEdit::textChanged, this, &KBStatementWidget::setValue);
    connect(objectCombo, &QComboBox::currentTextChanged, this, &KBStatementWidget::setObject);
    connect(attributeCombo, &QComboBox::currentTextChanged, this, &KBStatementWidget::setAttribute);
    connect(beliefSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KBStatementWidget::setBelief);
    connect(probabiltySpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KBStatementWidget::setProbabilty);
    connect(accuracySpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &KBStatementWidget::setAccuracy);
}

void KBStatementWidget::fillObjectsCombo()
{
    objectCombo->clear();
    QStringList objectNames;
    for (auto it = m_kb->objects.begin(); it != m_kb->objects.end(); it++) {
        objectNames << it->m_name;
    }
    objectCombo->addItems(objectNames);

    if (m_statement->m_objectName != "") {
        objectCombo->setCurrentIndex(objectCombo->findText(m_statement->m_objectName));
    }
    setObject(objectCombo->currentText());
}

void KBStatementWidget::fillAttributeCombo()
{
    attributeCombo->clear();
    QStringList attributeNames;
    KBObject* obj = m_kb->getObject(m_statement->m_objectName);
    if (obj) {
        for (auto it = obj->m_attributes.begin(); it != obj->m_attributes.end(); it++) {
            attributeNames << it->m_name;
        }
    }
    attributeCombo->addItems(attributeNames);
    if (m_statement->m_attributeName != "") {
        attributeCombo->setCurrentIndex(attributeCombo->findText(m_statement->m_attributeName));
    }
    setAttribute(attributeCombo->currentText());
}

void KBStatementWidget::setObject(const QString name)
{
    KBObject* obj = m_kb->getObject(name);
    if (obj) {
        m_statement->m_objectName = name;
    }
    fillAttributeCombo();
    emit changed();
}

void KBStatementWidget::setAttribute(const QString name)
{
    KBObject* obj = m_kb->getObject(m_statement->m_objectName);
    if (obj && obj->getAttribute(name)) {
        m_statement->m_attributeName = name;
    }
    emit changed();
}

void KBStatementWidget::setValue(const QString value)
{
    m_statement->m_value = value;
    emit changed();
}

void KBStatementWidget::setBelief(int value)
{
    m_statement->m_belief = value;
    emit changed();
}

void KBStatementWidget::setProbabilty(int value)
{
    m_statement->m_probabilty = value;
    emit changed();
}

void KBStatementWidget::setAccuracy(int value)
{
    m_statement->m_accuracy = value;
    emit changed();
}
