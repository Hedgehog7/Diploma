#include "KBCreateObjectDialog.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QTextEdit>
#include <QFormLayout>

#include "AKBEditor.h"
#include "KBAttributeScrollItem.h"
#include "KBAttributeScrollHeader.h"
#include "KBAttribute.h"

KBCreateObjectDialog::KBCreateObjectDialog(KBObject *object, QWidget *parent)
	: QDialog(parent),
    m_object(object),
    nameEdit(new QLineEdit),
	attributeScroll(new QScrollArea),
    scrollLayout(new QVBoxLayout),
    okBtn(new QPushButton("Ok"))
{
    qDebug() << "Enter KBCreateObjectDialog";
    buildUi();
}

void KBCreateObjectDialog::buildUi() {
    auto mainLayout = new QVBoxLayout;
    auto nameLabel = new QLabel("Name");

    auto scrollWidget = new QWidget;
    auto scrollHeader = new KBAttributeScrollHeader();
    scrollWidget->setLayout(scrollLayout);

    attributeScroll->setWidget(scrollWidget);
    attributeScroll->setWidgetResizable(true);

    scrollLayout->addWidget(scrollHeader);
    scrollLayout->setAlignment(Qt::AlignTop);
    scrollLayout->setAlignment(scrollHeader, Qt::AlignTop);


    auto fieldsLayout = new QVBoxLayout;

    fieldsLayout->addWidget(nameLabel);
    fieldsLayout->addWidget(nameEdit);
    mainLayout->addLayout(fieldsLayout);

    auto buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(okBtn);

    mainLayout->addWidget(attributeScroll);
    mainLayout->addLayout(buttonsLayout);

    this->setLayout(mainLayout);

    nameEdit->setText(m_object->m_name);
    fillAttributesScroll();
    okBtn->setEnabled(scrollLayout->count() > 1 && m_object->m_name != "");

    connect(okBtn, &QPushButton::clicked, this, &KBCreateObjectDialog::beforeClose);
    connect(nameEdit, &QLineEdit::textChanged, this, &KBCreateObjectDialog::setName);
    connect(scrollHeader, &KBAttributeScrollHeader::add, this, &KBCreateObjectDialog::addAttribute);
}

void KBCreateObjectDialog::fillAttributesScroll()
{
    for (auto it = m_object->m_attributes.begin(); it != m_object->m_attributes.end(); it++) {
        auto item = new KBAttributeScrollItem(&(*it));
        scrollLayout->addWidget(item);
        connect(item, &KBAttributeScrollItem::remove, this, &KBCreateObjectDialog::removeAttribute);
    }
}

void KBCreateObjectDialog::fillObject()
{
//    m_object->m_attributes.clear();
    for (int i = 1; i < scrollLayout->count(); i++) {
        KBAttributeScrollItem* item = (KBAttributeScrollItem* ) scrollLayout->itemAt(i)->widget();
        m_object->addAttribute(*item->m_attribute);
    };
}

void KBCreateObjectDialog::beforeClose() {
    fillObject();
	this->accept();
}

KBCreateObjectDialog::~KBCreateObjectDialog()
{
}

void KBCreateObjectDialog::setName(const QString name)
{
    okBtn->setEnabled(scrollLayout->count() > 1 && m_object->m_name != "");
    m_object->m_name = name;
}

void KBCreateObjectDialog::addAttribute()
{
    KBAttribute* attribute = new KBAttribute(QString("Attribute" + QString::number(scrollLayout->count() - 1)), "");
    auto item = new KBAttributeScrollItem(attribute);
    scrollLayout->addWidget(item);
    okBtn->setEnabled(scrollLayout->count() > 1 && m_object->m_name != "");
    connect(item, &KBAttributeScrollItem::remove, this, &KBCreateObjectDialog::removeAttribute);
}

void KBCreateObjectDialog::removeAttribute()
{
    qDebug() << "removeAttributeWidget";
	if (!sender()) {
		return;
	}
	QWidget* sender_widget = (QWidget*) sender();
    qDebug() << "Sender: " << sender_widget;
    for (int i = 0; i < scrollLayout->count(); i++) {
		auto item = scrollLayout->itemAt(i);
        auto widget = item->widget();
        qDebug() << "Current widget " << widget;
        if (widget == sender_widget) {
            qDebug() << "DELETE THIS!!!";
            m_object->deleteAttribute(qobject_cast<KBAttributeScrollItem*>(widget)->m_attribute->m_name);
            scrollLayout->removeWidget(widget);
            scrollLayout->removeItem(item);
		}
	};
    okBtn->setEnabled(scrollLayout->count() > 1 && m_object->m_name != "");
}
