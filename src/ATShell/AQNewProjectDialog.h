#ifndef AQNEWPROJECTDIALOG_H
#define AQNEWPROJECTDIALOG_H

#include <QDialog>
#include <QWizard>
#include "ui_AQNewProjectDialog.h"

class AProjectTemplate;

class AQNewProjectDialog : public QDialog
{
	Q_OBJECT

public:
	AQNewProjectDialog(QWidget *parent = 0);
	~AQNewProjectDialog();
	void openProjectDirectory();
	QString projectDir();
	QString projectName();
private:
	Ui::AQNewProjectDialog ui;
};

class QListWidget;


class path_edit_widget : public QWidget
{
	Q_OBJECT

public:
	path_edit_widget(QWidget * parent = nullptr);
	void setPath(const QString & path);
	QLineEdit * edPath;

signals:
	void pathChanged(const QString&);
};

class ANewProjectWizard : public QWizard
{
	Q_OBJECT

public:
	ANewProjectWizard();
	~ANewProjectWizard();
	void addTemplate(const QString & name, AProjectTemplate * templ);

	QListWidget * listTemplates;

	QString projectDir, projectName;
	std::vector<AProjectTemplate*> templates;
	AProjectTemplate * selectedTemplate;
/*protected:
	virtual void done(int result);
signals:
	void finished(const QString & template_name, const QString & project_dir, const QString & project_name);*/
};

#endif // AQNEWPROJECTDIALOG_H
