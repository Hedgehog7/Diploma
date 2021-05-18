#include "FormulaWidget.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
const char* FormulaWidget::OPERATIONS_STRING = "eq gt lt ne";

FormulaWidget::FormulaWidget(KnowledgeBase* kb, QWidget *parent)
    : QWidget(parent),
    objectCombo(new QComboBox),
    attributeCombo(new QComboBox),
    opCombo(new QComboBox),
    valueEdit(new QLineEdit),
    m_formula(new Formula)
{
    qDebug() << "FormulaWidget Constructor";
    m_kb = kb;
    buildUi();
}

FormulaWidget::FormulaWidget(Formula *formula, KnowledgeBase *kb, QWidget *parent)
    :QWidget(parent),
      objectCombo(new QComboBox),
      attributeCombo(new QComboBox),
      opCombo(new QComboBox),
      valueEdit(new QLineEdit),
      m_formula(formula)
{
    qDebug() << "FormulaWidget Formula Constructor";
    m_kb = kb;
    buildUi();
}

bool FormulaWidget::isValid()
{
    return m_formula->m_attribute.m_name != "" && m_formula->m_object.m_name != "" && m_formula->m_op != "" && m_formula->m_value != "";
}

void FormulaWidget::buildUi() {
    qDebug() << "Enter Formula Widget Build UI";
    auto layout = new QVBoxLayout;
    auto objectLabel = new QLabel("Object");
    layout->addWidget(objectLabel);
    layout->addWidget(objectCombo);

    auto attributeLabel = new QLabel("Attribute");
    layout->addWidget(attributeLabel);
    layout->addWidget(attributeCombo);

    auto opLabel = new QLabel("Operation");
    layout->addWidget(opLabel);
    layout->addWidget(opCombo);

    auto valueLabel = new QLabel("Value");
    layout->addWidget(valueLabel);
    layout->addWidget(valueEdit);

    this->setLayout(layout);
    valueEdit->setText(m_formula->m_value);

    fillObjectsCombo();
    fillAttributeCombo();
    fillOpCombo();
    connect(objectCombo, &QComboBox::currentTextChanged, this, &FormulaWidget::setObject);
    connect(attributeCombo, &QComboBox::currentTextChanged, this, &FormulaWidget::setAttribute);
    connect(opCombo, &QComboBox::currentTextChanged, this, &FormulaWidget::setOp);
    connect(valueEdit, &QLineEdit::textChanged, this, &FormulaWidget::setValue);
    qDebug() << "Leave Formula Widget BUildUi";
}

void FormulaWidget::fillObjectsCombo()
{
    objectCombo->clear();
    QStringList objectNames;
    for (auto it = m_kb->objects.begin(); it != m_kb->objects.end(); it++) {
        objectNames << it->m_name;
    }
    objectCombo->addItems(objectNames);

    if (m_formula->m_object.m_name != "") {
        objectCombo->setCurrentIndex(objectCombo->findText(m_formula->m_object.m_name));
    }
    setObject(objectCombo->currentText());
}

void FormulaWidget::fillAttributeCombo()
{
    attributeCombo->clear();
    QStringList attributeNames;
    for (auto it = m_formula->m_object.m_attributes.begin(); it != m_formula->m_object.m_attributes.end(); it++) {
        attributeNames << it->m_name;
    }

    attributeCombo->addItems(attributeNames);
    if (m_formula->m_attribute.m_name != "") {
        attributeCombo->setCurrentIndex(attributeCombo->findText(m_formula->m_attribute.m_name));
    }
    setAttribute(attributeCombo->currentText());
}

void FormulaWidget::fillOpCombo()
{
    opCombo->clear();
    opCombo->addItems(QStringList() << "=" << "<" << ">" << "!=" << ">=" << "<=");

    if (m_formula->m_op != "") {
        opCombo->setCurrentIndex(opCombo->findText(m_formula->m_op));
    }
    setOp(opCombo->currentText());
}

void FormulaWidget::setObject(QString objectname)
{
    m_formula->m_object = *m_kb->getObject(objectname);
    fillAttributeCombo();
    emit formulaChanged();
}

void FormulaWidget::setOp(QString op)
{
    m_formula->m_op = op;
    emit formulaChanged();
}

void FormulaWidget::setValue(QString value)
{
    m_formula->m_value = value;
    emit formulaChanged();
}

void FormulaWidget::setAttribute(const QString attributeName)
{
    if (m_formula->m_object.getAttribute(attributeName)) {
        m_formula->m_attribute = *m_formula->m_object.getAttribute(attributeName);
        emit formulaChanged();
    };
}

