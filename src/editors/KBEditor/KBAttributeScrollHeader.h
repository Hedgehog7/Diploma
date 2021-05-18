#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class KBAttributeScrollHeader :
	public QWidget
{
	Q_OBJECT
public:
	KBAttributeScrollHeader();
	~KBAttributeScrollHeader();


signals:
	void add();

private slots:
	void onAddBtnClick();

private:
	const static char* ADD_BTN_TEXT;
	const static char* NAME_LABEL_TEXT;
	const static char* TYPE_LABEL_TEXT;
	QHBoxLayout* layout;
	QLabel* nameLabel;
	QLabel* typeLabel;
	QPushButton* addBtn;
	
};

