#pragma once

#include "word.h"

#include <iostream>
#include <map>
#include <vector>

namespace linguistic_processor {
namespace morphological_analysis {

class Dict {
public:
    Dict(std::istream& inputStream);

    // FIXME: Check errors.
    const Lemma& findLemma(std::string str) const {
        const size_t i = index_.find(str)->second;
        return lemmas_[i];
    }

    const std::vector<Lemma>& lemmas() const {
        return lemmas_;
    }

private:
    std::map<std::string, size_t> index_;
    std::vector<Lemma> lemmas_;
};

} // namespace morphological_analysis
} // namespace linguistic_processor
