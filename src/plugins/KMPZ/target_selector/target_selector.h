#pragma once

#include <QString>

#include <memory>

namespace NAT {
namespace NMA {

class TTargetSelector {
public:
    TTargetSelector(const QString& db);

    QString Select(const QString& text) const;

    ~TTargetSelector();

private:
    TTargetSelector(const TTargetSelector&) = delete;
    TTargetSelector& operator=(const TTargetSelector&) = delete;

    class TImpl;
    std::unique_ptr<TImpl> Impl_;
};

} // namespace NMA
} // namespace NAT
