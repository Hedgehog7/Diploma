
#include "AProjectTemplate.h"
#include <ATCore/check.h>
#include <ATCore/utils/xml_helpers.h>
#include <QFileInfo>
#include <QDir>
#include <iostream>
#include <libxml/tree.h>

AProjectTemplate::AProjectTemplate()
{}

void AProjectTemplate::load(const QString & fname)
{
	std::cout << "Loading template from " << fname.toStdString() << std::endl;

	QFileInfo fi(fname);
	dirPath = fi.dir().absolutePath();

	int r = read_xml_doc(fname.toStdString(), [=](_xmlNode * node_ptr) {
		this->name = QString::fromUtf8((const char *)xmlGetProp(node_ptr, BAD_CAST "name"));

		xmlNode * docs_node = child_for_path(node_ptr, "documents");
		xml_for_each_child(docs_node, doc_node)	{
			std::string doc_rel_path((char*)xmlGetProp(doc_node, BAD_CAST "path"));
			docs.push_back(QString::fromStdString(doc_rel_path));
		}
	});

	perform_check(r == 0);
}
