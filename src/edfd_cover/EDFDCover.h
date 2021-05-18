#ifndef EDFDCOVER_H
#define EDFDCOVER_H

#include <vector>
#include "APlan.h"

class EDFDDocument;
class APlan;
class SDP;
class AProject;

class EDFDCover
{
public:
    //! Launch plan generation
    //! @param proj - project
    //! @param common_dfd - merged EDFDs representing project architecture
    //! @param sdps - set of SDP (or TPP) to use for architecture cover
    //! @returns generated plan
	EDFD_COVER_API static APlan* buildPlan(AProject* proj, const EDFDDocument * common_dfd, const std::vector<SDP>& sdps);
};

#endif // EDFDCOVER_H
