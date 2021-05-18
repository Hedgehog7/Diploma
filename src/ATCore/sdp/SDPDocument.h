#ifndef SDPDOCUMENT_H
#define SDPDOCUMENT_H

#include "../ADocument.h"
#include <vector>

#include "SDP.h"

class AT_CORE_API SDPDocument : public AXMLDocument
{
public:
    virtual void serialize(_xmlNode * document_node) const override;
    virtual AError deserialize(_xmlNode * document_node) override;
    virtual Type type() const override;

    const std::vector<SDP>& getSDPs() const;
private:
    std::vector<SDP> _sdps;

    SDP deserialize_sdp(_xmlNode* procedure_node) const;
    SDP::Node deserialize_sdp_node(_xmlNode* node_node) const;
    SDP::Flow deserialize_sdp_flow(_xmlNode* flow_node) const;
    SDP::Fragment deserialize_sdp_fragment(_xmlNode* fragment_node) const;
    SDP::Task deserialize_sdp_task(_xmlNode* task_node) const;
};

#endif // SDPDOCUMENT_H
