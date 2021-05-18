#include <iostream>
#include <string>
#include <ATProject/AProject.h>
#include <QDir>
#include <ATCore/utils/xml_helpers.h>
#include <ATCore/utils/helpers.h>
#include <ATProject/ADocumentRef.h>



using namespace std;

void copyAndCloseFile(QFile * from, QFile * to);
AProject * AtprjToAProject(string prj_path);

int main(int argc, char *argv[])
{
	QString path = argv[1];
	path = path.left(path.lastIndexOf('/'));
	// Checking if path is exists
	if (!QDir(path).exists())
		return -1;

	// Creating $(path)/release directory
	QDir(path).mkdir("release");
	QFile atprj(argv[1]);

	// Copying es.exe file.
	Q_INIT_RESOURCE(res); //resource init
	QFile es(":/resources/es.exe");
	QFile se(path + "/release/" + QFileInfo(atprj).baseName().toStdString().c_str() + ".exe");
	copyAndCloseFile(&es, &se);
	atprj.close();

	// Deserialization .atprj
	string prj_path(argv[1]);
	AProject * prj = AtprjToAProject(prj_path);
	for (auto ref : prj->documents) {
		QString qref(ref->absolutePath());
		QFile from(qref);
		QFile to(QString(path + "/release") + qref.right(qref.length() - qref.lastIndexOf("/")));
		copyAndCloseFile(&from, &to);
	}

	// Generating config. Too Weird. Better to do it another way.
	QFile config(path + QString("/release/config.xml"));
	config.open(QIODevice::WriteOnly);
	int dcounter = 1, kcounter = 1, vcounter = 1;
	config.write("<config>\n\n");
	for (auto ref : prj->documents) {
		QString qref(ref->absolutePath());
		QString fname = qref.right(qref.length() - qref.lastIndexOf("/") - 1);
		QString fext = fname.right(qref.length() - qref.lastIndexOf(".") - 1);
		if (fext.compare(QString("dsf")) == 0) {
			string s = "<object Name='DSDLEditor" + ((dcounter > 1) ? to_string(dcounter) : "") + "' Class='DSDLEditorLib.DSDLEditor'>\n";
			config.write(s.c_str());
			config.write("<config>\n");
			s = "<FileName>" + fname.toStdString() + "</FileName>\n";
			config.write(s.c_str());
			config.write("</config>\n</object>\n\n");
			s = "<object Name='Dialoger" + ((dcounter > 1) ? to_string(dcounter) : "") + "' Class='DialogerLib.Dialoger'>\n";
			config.write(s.c_str());
			config.write("<config>\n");
			s = "<FileName>" + fname.toStdString() + "</FileName>\n";
			config.write(s.c_str());
			config.write("</config>\n</object>\n\n");
			dcounter++;
		}
		if (fext.compare(QString("kbs")) == 0) {
			string s = "<object Name='ESKernel" + ((kcounter > 1) ? to_string(kcounter) : "") + "' Class='EsRunTime.CoESRunTime'>\n";
			config.write(s.c_str());
			config.write("<config>\n");
			s = "<FileName>" + fname.toStdString() + "</FileName>\n";
			config.write(s.c_str());
			config.write("</config>\n</object>\n\n");
			s = "<object Name='KBEditor" + ((kcounter > 1) ? to_string(kcounter) : "") + "' Class='kbtools.AutoKBEditor'>\n";
			config.write(s.c_str());
			config.write("<config>\n");
			s = "<FileName>" + fname.toStdString() + "</FileName>\n";
			config.write(s.c_str());
			config.write("</config>\n</object>\n\n");
			kcounter++;
		}
		if (fext.compare(QString("vbs")) == 0) {
			string s = "<object Name='Scripter" + ((vcounter > 1) ? to_string(vcounter) : "") + "' Class='ScriptLib.Runner'>\n";
			config.write(s.c_str());
			config.write("<config>\n<script Timeout=\" - 1\" Language=\"VBScript\" Silent=\"false\"/>");
			s = "<FileName>" + fname.toStdString() + "</FileName>\n";
			config.write(s.c_str());
			config.write("</config>\n</object>\n\n");
			vcounter++;
		}		
	}
	string s = "<message RcvrName='Dialoger" + ((dcounter > 2) ? to_string(dcounter) : "") + "'>\n";
	config.write(s.c_str());
	config.write("<message ProcName='Run'>\n</message>\n<message>\n\n</config>");
	config.close();

	return 0;
}

void copyAndCloseFile(QFile * from, QFile * to) {
	if (QFileInfo(*from).completeSuffix() == "edfd") return;
	if (from->open(QIODevice::ReadOnly) && to->open(QIODevice::WriteOnly))
		to->write(from->readAll());
	to->close();
	from->close();
}

AProject * AtprjToAProject(string prj_path) {
	AProject * prj = nullptr;
	int r = read_xml_doc(prj_path, [&](xmlNode* node_ptr) {
		prj = new AProject();
		string dir_path, fname;
		split_file_path(prj_path, dir_path, fname);
		prj->fileName = QString::fromStdString(fname);
		prj->dirPath = QString::fromStdString(dir_path);
		prj->deserialize(node_ptr);
	});
	return prj;
}