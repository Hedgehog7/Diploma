
#ifndef ATEnvironment_ADocument_h
#define ATEnvironment_ADocument_h

#include "config.h"
#include "AError.h"

class _xmlNode;

class AT_CORE_API ADocument
{
public:
    enum class Type {EDFD, ArchitectureMaket, SDP, NotSystem};

	const std::string & fileName() const;

	AError save();
	AError save(std::string path);
	virtual AError saveToFile(const std::string & file_name);
	virtual AError loadFromFile(const std::string & file_name);

	virtual Type type() const;
protected:
	std::string mFileName, mDirectory;
};

class AT_CORE_API AXMLDocument : public ADocument
{
	using _Base = ADocument;
public:
	virtual AError saveToFile(const std::string & file_name);
	virtual AError loadFromFile(const std::string & file_name);

	virtual void serialize(_xmlNode * document_node) const = 0;
	virtual AError deserialize(_xmlNode * document_node) = 0;
};

#endif
