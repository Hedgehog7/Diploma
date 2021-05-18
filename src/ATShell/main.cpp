#include "ATApplication.h"
#include "ATEnvironment.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    //Здесь начинается харкод для заполнения рабочей папки нужными файлами
    //При лишнем времени и желании можно добавить обработчик с условиями
    //system("chmod +x ../../../resources/support_skripts");
    system("./../../../resources/support_skripts/create_ref.sh");
    system("./../../../resources/support_skripts/update_procedures.sh");
    system("./../../../resources/support_skripts/update_lib.sh");

    //Здесь хардкод заканчивается
	ATApplication a(argc, argv);
	ATEnvironment w(&a);
	//w.show();
	w.showMaximized();

	if(argc > 1)
	{
		QString project_path(argv[1]);
		qDebug() << "Opening project " << project_path;

		w.openProject(project_path.toStdString());
	}

	return a.exec();
}

//D:\Projects\AT-PLANNER\test_projects\SimpleIES\SimpleIES.atprj
