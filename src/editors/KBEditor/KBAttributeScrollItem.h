#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "KBAttribute.h"
class KBAttributeScrollItem :
	public QWidget
{
	Q_OBJECT
public:
    KBAttributeScrollItem(KBAttribute* attribute);
	~KBAttributeScrollItem();
	
    KBAttribute* m_attribute;
signals:
	void remove();

private slots:
    void setName(const QString name);
    void setType(const QString type);
	void onClose();

private:
	const static char* DEL_BTN_TEXT;
	QHBoxLayout* layout;
	QPushButton* delBtn;
	QLineEdit* nameEdit;
	QComboBox* typeCombo;

    void buildUi();
    void fillTypeCombo();
};

