#pragma once
#include <QLineEdit>
#include <QComboBox>
#include <QString>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>

#include "KBObject.h"
#include "KBAttribute.h"
class KBCreateObjectDialog : public QDialog
{
	Q_OBJECT

public:
    KBCreateObjectDialog(KBObject* object, QWidget *parent = 0);
	~KBCreateObjectDialog();
    KBObject* m_object;

private slots:
	void beforeClose();
    void setName(const QString name);
    void addAttribute();
    void removeAttribute();

private:
    QLineEdit* nameEdit;
    QPushButton* okBtn;
	QScrollArea* attributeScroll;
	QVBoxLayout* scrollLayout;

    void buildUi();
    void fillAttributesScroll();
    void fillObject();
};
