#include "KBAttributeScrollItem.h"
#include <QDebug>

const char* KBAttributeScrollItem::DEL_BTN_TEXT = "Del";

KBAttributeScrollItem::KBAttributeScrollItem(KBAttribute* attribute)
	:layout(new QHBoxLayout),
	nameEdit(new QLineEdit),
	typeCombo(new QComboBox),
	delBtn(new QPushButton(DEL_BTN_TEXT)),
    m_attribute(attribute)
{
    qDebug() << "Enter KBAttributeScrollItem";
    buildUi();
}

void KBAttributeScrollItem::buildUi() {
    this->setLayout(layout);

    layout->addWidget(nameEdit);
    layout->addWidget(typeCombo);
    layout->addWidget(delBtn);
    nameEdit->setText(m_attribute->m_name);
    fillTypeCombo();

    connect(delBtn, &QPushButton::clicked, this, &KBAttributeScrollItem::onClose);
    connect(nameEdit, &QLineEdit::textChanged, this, &KBAttributeScrollItem::setName);
    connect(typeCombo, &QComboBox::currentTextChanged, this, &KBAttributeScrollItem::setType);
}

void KBAttributeScrollItem::fillTypeCombo()
{
    typeCombo->addItems(QStringList() << "String" << "Number");
    if (m_attribute->m_type != "")
        typeCombo->setCurrentIndex(typeCombo->findText(m_attribute->m_type));
    m_attribute->m_type= typeCombo->currentText();
}

KBAttributeScrollItem::~KBAttributeScrollItem()
{}


void KBAttributeScrollItem::setName(const QString name)
{
    qDebug() << "Enter KBAttributeScrollItem::setName";
    qDebug() << "Name " << name;
    m_attribute->m_name = name;
    qDebug() << "Attr Name " << m_attribute->m_name;
}
void KBAttributeScrollItem::setType(const QString type)
{
    m_attribute->m_type = type;
}

void KBAttributeScrollItem::onClose()
{
    qDebug() << "onClose";
    emit remove();
	this->close();
}
