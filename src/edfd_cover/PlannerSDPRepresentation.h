#ifndef PLANNERSDPREPRESENTATION_H
#define PLANNERSDPREPRESENTATION_H

#include "edfd_cover.h"
#include <vector>

class SDP;

class PlannerSDPRepresentation
{
public:
    PlannerSDPRepresentation(const SDP& sdp);

    using fragment_id_t = std::size_t;
    using fragment_edfd_t = std::pair<edfd_graph, fragment_id_t>;

    const edfd_graph& getMainPartEDFD() const;
    const fragment_edfd_t getMainPart() const;
    const std::vector<fragment_edfd_t>& getOptionalParts() const;
    const SDP& getOriginalSDP() const;
private:
    const SDP& originalSDP;
    fragment_edfd_t mainPart;
    std::vector<fragment_edfd_t> optionalParts;

    void convertSDPToGraphs(const SDP& sdp);
};

#endif // PLANNERSDPREPRESENTATION_H
