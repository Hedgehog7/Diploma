#include "KBAttributeScrollHeader.h"


const char* KBAttributeScrollHeader::NAME_LABEL_TEXT = "Name";
const char* KBAttributeScrollHeader::TYPE_LABEL_TEXT = "Type";
const char* KBAttributeScrollHeader::ADD_BTN_TEXT = "Add";

KBAttributeScrollHeader::KBAttributeScrollHeader()
	:layout(new QHBoxLayout),
	addBtn(new QPushButton),
	nameLabel(new QLabel(NAME_LABEL_TEXT)),
	typeLabel(new QLabel(TYPE_LABEL_TEXT))
{
	this->setLayout(layout);

	QPushButton* addBtn = new QPushButton("Add");
	layout->addWidget(nameLabel);
	layout->addWidget(typeLabel);
	layout->addWidget(addBtn);
	connect(addBtn, &QPushButton::clicked, this, &KBAttributeScrollHeader::onAddBtnClick);
}

void KBAttributeScrollHeader::onAddBtnClick()
{
	emit add();
}

KBAttributeScrollHeader::~KBAttributeScrollHeader()
{
}
