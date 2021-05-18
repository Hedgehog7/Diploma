#include "RuleWidget.h"
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>

RuleWidget::RuleWidget(KBRule *rule, KnowledgeBase *kb, QWidget *parent)
    :QWidget(parent),
     m_kb(kb),
     m_rule(rule),
     nameEdit(new QLineEdit),
     arg1Combo(new QComboBox),
     arg2Combo(new QComboBox),
     temporalRelCombo(new QComboBox)
{
    qDebug() << "Enter RuleWidget constructor";
    buildUi();

}

bool RuleWidget::isValid()
{
}

void RuleWidget::buildUi()
{
//    auto mainLayout = new QVBoxLayout;
//    auto nameLabel = new QLabel("Name");
//    mainLayout->addWidget(nameLabel);
//    mainLayout->addWidget(nameEdit);
//    auto ifLabel = new QLabel("IF");
//    mainLayout->addWidget(ifLabel);

//    auto conditionLayout = new QHBoxLayout;
//    conditionLayout->addWidget(arg1Combo);
//    conditionLayout->addWidget(temporalRelCombo);
//    conditionLayout->addWidget(arg2Combo);
//    mainLayout->addLayout(conditionLayout);

//    auto thenLabel = new QLabel("THEN");
//    mainLayout->addWidget(thenLabel);

//    auto thenLayout = new QHBoxLayout;
//    thenLayout->addWidget(objectCombo);
//    auto dotLabel = new QLabel(".");
//    thenLayout->addWidget(dotLabel);
//    thenLayout->addWidget(attributeCombo);
//    auto assignLabel = new QLabel("=");
//    thenLayout->addWidget(assignLabel);
//    thenLayout->addWidget(valueEdit);
//    mainLayout->addLayout(thenLayout);

//    auto withLabel = new QLabel("WITH");
//    mainLayout->addWidget(withLabel);

//    auto formLayout = new QFormLayout;
//    auto beliefLabel = new QLabel("Belief");
//    formLayout->addRow(beliefLabel, beliefSpin);
//    auto probabiltyLabel = new QLabel("Probabilty");
//    formLayout->addRow(probabiltyLabel, probabiltySpin);
//    auto accuracyLabel = new QLabel("Accuracy");
//    formLayout->addRow(accuracyLabel, accuracySpin);

//    mainLayout->addLayout(formLayout);
//    this->setLayout(mainLayout);
//    fillArgsCombo();
//    fillObjectsCombo();
//    fillAttributeCombo();
//    fillTemporalRelCombo();

//    nameEdit->setText(m_rule->m_name);
//    valueEdit->setText(m_rule->m_value);
//    beliefSpin->setValue(m_rule->m_belief);
//    probabiltySpin->setValue(m_rule->m_probabilty);
//    accuracySpin->setValue(m_rule->m_accuracy);

//    connect(nameEdit, &QLineEdit::textChanged, this, &RuleWidget::setName);
//    connect(valueEdit, &QLineEdit::textChanged, this, &RuleWidget::setValue);
//    connect(objectCombo, &QComboBox::currentTextChanged, this, &RuleWidget::setObject);
//    connect(attributeCombo, &QComboBox::currentTextChanged, this, &RuleWidget::setAttribute);
//    connect(arg1Combo, &QComboBox::currentTextChanged, this, &RuleWidget::setArg1);
//    connect(arg2Combo, &QComboBox::currentTextChanged, this, &RuleWidget::setArg2);
//    connect(temporalRelCombo, &QComboBox::currentTextChanged, this, &RuleWidget::setTemporalRel);
//    connect(beliefSpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RuleWidget::setBelief);
//    connect(probabiltySpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RuleWidget::setProbabilty);
//    connect(accuracySpin, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &RuleWidget::setAccuracy);
}
void RuleWidget::fillObjectsCombo()
{
//    objectCombo->clear();
//    QStringList objectNames;
//    for (auto it = m_kb->objects.begin(); it != m_kb->objects.end(); it++) {
//        objectNames << it->m_name;
//    }
//    objectCombo->addItems(objectNames);

//    if (m_rule->m_objectName != "") {
//        objectCombo->setCurrentIndex(objectCombo->findText(m_rule->m_objectName));
//    }
//    setObject(objectCombo->currentText());
}

void RuleWidget::fillAttributeCombo()
{
//    attributeCombo->clear();
//    QStringList attributeNames;
//    KBObject* obj = m_kb->getObject(m_rule->m_objectName);
//    if (obj) {
//        for (auto it = obj->m_attributes.begin(); it != obj->m_attributes.end(); it++) {
//            attributeNames << it->m_name;
//        }
//    }
//    attributeCombo->addItems(attributeNames);
//    if (m_rule->m_attributeName != "") {
//        attributeCombo->setCurrentIndex(attributeCombo->findText(m_rule->m_attributeName));
//    }
//    setAttribute(attributeCombo->currentText());
}

void RuleWidget::fillTemporalRelCombo(){
//    QStringList sl;
//    sl << "b" << "bi" << "m" << "mi" << "o" << "oi" << "d" << "di" << "s" << "si" << "f" << "fi" << "E";
//    temporalRelCombo->addItems(sl);
//    if (m_rule->m_temporalRelation != "") {
//        temporalRelCombo->setCurrentIndex(temporalRelCombo->findText(m_rule->m_temporalRelation));
//    }
//    setTemporalRel(temporalRelCombo->currentText());

}

void RuleWidget::setArg1(const QString name)
{
//    m_rule->m_arg1 = name;
//    emit ruleChanged();
}

void RuleWidget::setArg2(const QString name)
{
//    m_rule->m_arg2 = name;
//    emit ruleChanged();
}

void RuleWidget::setTemporalRel(const QString name)
{
//    m_rule->m_temporalRelation = name;
//    emit ruleChanged();
}

//void RuleWidget::setObject(const QString name)
//{
//    KBObject* obj = m_kb->getObject(name);
//    if (obj) {
//        m_rule->m_objectName = name;
//    }
//    fillAttributeCombo();
//    emit ruleChanged();
//}

//void RuleWidget::setAttribute(const QString name)
//{
//    KBObject* obj = m_kb->getObject(m_rule->m_objectName);
//    if (obj && obj->getAttribute(name)) {
//        m_rule->m_attributeName = name;
//    }
//    emit ruleChanged();

//}

//void RuleWidget::setValue(const QString value)
//{
//    m_rule->m_value = value;
//    emit ruleChanged();
//}

//void RuleWidget::setBelief(int value)
//{
//    m_rule->m_belief = value;
//    emit ruleChanged();
//}

//void RuleWidget::setProbabilty(int value)
//{
//    m_rule->m_probabilty = value;
//    emit ruleChanged();
//}

//void RuleWidget::setAccuracy(int value)
//{
//    m_rule->m_accuracy = value;
//    emit ruleChanged();
//}

void RuleWidget::setName(const QString name)
{
    m_rule->m_name = name;
    emit ruleChanged();
}
