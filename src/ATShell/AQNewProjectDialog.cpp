#include "AQNewProjectDialog.h"
#include <QtWidgets/QFileDialog>
#include <ATProject/AProjectTemplate.h>
#include <QListWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QStandardPaths>
#include <iostream>

Q_DECLARE_METATYPE(AProjectTemplate*)

AQNewProjectDialog::AQNewProjectDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.bnOK, &QPushButton::clicked, this, &AQNewProjectDialog::accept);
	connect(ui.bnCancel, &QPushButton::clicked, this, &AQNewProjectDialog::close);
	connect(ui.bnSelectDirectory, &QPushButton::clicked, this, &AQNewProjectDialog::openProjectDirectory);
}

AQNewProjectDialog::~AQNewProjectDialog()
{

}

void AQNewProjectDialog::openProjectDirectory()
{
	QString str = QFileDialog::getExistingDirectory(this, "Select project directory", ui.edDirectory->text());

	if(str != "")
		ui.edDirectory->setText(str);
}

QString AQNewProjectDialog::projectDir()
{
	return ui.edDirectory->text();
}

QString AQNewProjectDialog::projectName()
{
	return ui.edProjectName->text();
}

path_edit_widget::path_edit_widget(QWidget * parent)
{

	auto lyt = new QHBoxLayout;

	edPath = new QLineEdit(this);
	edPath->setMinimumWidth(300);
	
	

	auto bnSelectDir = new QPushButton("...", this);

	//std::cout << locs[0].toStdString() << std::endl;
	//
	
	lyt->addWidget(edPath);
	lyt->addWidget(bnSelectDir);

	setLayout(lyt);


	connect(bnSelectDir, &QPushButton::clicked, [=](){
			QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), edPath->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
			if(!dir.isEmpty())
				setPath(dir);
		});
}

void path_edit_widget::setPath(const QString & path)
{
	edPath->setText(path);
	emit pathChanged(path);
}


ANewProjectWizard::ANewProjectWizard()
{
	QDir templ_dir(QDir::currentPath() + "/templates");
	std::cout << "Scanning template from " << templ_dir.absolutePath().toStdString() << std::endl;
	
	auto dirs = templ_dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	for (auto & entr : dirs)
	{
		std::cout << "Loading template from " << entr.filePath().toStdString() << std::endl;

		QDir cur_dir(entr.filePath());
		cur_dir.setNameFilters(QStringList() << "*.tmpl");
		QStringList fileList = cur_dir.entryList();
		if(!fileList.empty())
		{
			
			auto templ_file = fileList[0];

			QFileInfo fi(cur_dir, templ_file);

			auto new_templ = new AProjectTemplate;
			new_templ->load(fi.absoluteFilePath());
			templates.push_back(new_templ);
		}
	}

	std::cout << "Starting wizard" << std::endl;

	//Template selection
	auto templates_list = new QWizardPage;
	{
		templates_list->setTitle(tr("Template Selection"));

		auto lyt = new QVBoxLayout;

		auto lb = new QLabel(tr("Select Template"), templates_list);
		lyt->addWidget(lb);

		listTemplates = new QListWidget(templates_list);

		for(auto templ : templates)
		{
			addTemplate(templ->name, templ);
		}
		
		//addTemplate(tr("Dynamic Integrated Expert System"), "dynamic_ies");
		//addTemplate(tr("Tutoring Integrated Expert System"), "tutoring_ies");
		lyt->addWidget(listTemplates);

		templates_list->setLayout(lyt);
	}

	addPage(templates_list);

	//Name and location
	auto location_page = new QWizardPage;
	{
		auto lyt = new QFormLayout;

		auto edName = new QLineEdit(location_page);

		connect(edName, &QLineEdit::textChanged, [=](const QString &text){
			projectName = text;
		});
		
		lyt->addRow(tr("Project Name"), edName);

		auto locs = QStandardPaths::standardLocations(QStandardPaths::StandardLocation::DocumentsLocation);
		auto def_loc = locs[0];

		QString new_name;
		int i = 1;
		bool exists = true;
		do
		{
			new_name = QString("Unnamed_%1").arg(i++);
			QFileInfo fi(def_loc + "/" + new_name);
			exists = fi.exists();
		}while(exists);
		
		edName->setText(new_name);
		//edName->setText("Unnamed");

		auto edPath = new path_edit_widget(location_page);
		connect(edPath, &path_edit_widget::pathChanged, [=](const QString & _path){
			projectDir = _path;
		});

		edPath->setPath(def_loc);
		lyt->addWidget(edPath);

		lyt->addRow(tr("Location"), edPath);

		location_page->setLayout(lyt);		
	}
	addPage(location_page);

	connect(listTemplates, &QListWidget::itemClicked, [=](QListWidgetItem *item){
		//this->templateName = item->data(Qt::UserRole).toString();
		this->selectedTemplate = item->data(Qt::UserRole).value<AProjectTemplate*>();
	});
}

ANewProjectWizard::~ANewProjectWizard()
{
	for(auto tmpl : templates)
		delete tmpl;
}

void ANewProjectWizard::addTemplate(const QString & name, AProjectTemplate * templ)
{
	auto item = new QListWidgetItem(name);
	item->setData(Qt::UserRole, qVariantFromValue(templ));

	listTemplates->addItem(item);
}
/*
void ANewProjectWizard::done(int result)
{
	_Base::done(result);
	if(result == QDialog::Accepted)
	{
	void finished(const QString & template_name, const QString & project_dir, const QString & project_name);
}
*/
