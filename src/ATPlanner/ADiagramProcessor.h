
#ifndef APLANNER_ADiagramProcessor_H
#define APLANNER_ADiagramProcessor_H

#include <vector>

class EDFDDocument;

class ADiagramProcessor
{
public:
	EDFDDocument * mergeHierarchy(const std::vector<EDFDDocument*> & diagrams);
};

#endif
