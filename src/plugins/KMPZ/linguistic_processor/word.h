#pragma once

#include "grammeme.h"

#include <string>
#include <vector>
#include <iostream>

namespace linguistic_processor {
namespace morphological_analysis {

struct Word {
    std::string word;
    std::vector<Grammeme> grammeme;
};

std::ostream& operator << (std::ostream& out, const Word& w);

struct Lemma {
    size_t id;
    std::vector<Word> words;
};

std::ostream& operator << (std::ostream& out, const Lemma& l);

} // namespace morphological_analysis
} // namespace linguistic_processor
