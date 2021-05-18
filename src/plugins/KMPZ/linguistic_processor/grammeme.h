#pragma once

#include <string>

namespace linguistic_processor {
namespace morphological_analysis {

struct Grammeme {
    std::string parent;
    std::string name;
    std::string alias;
    std::string description;
};

Grammeme FindGrammeme(std::string str);

} // namespace morphological_analysis
} // namespace linguistic_processor
