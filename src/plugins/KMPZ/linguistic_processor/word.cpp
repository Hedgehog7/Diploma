#include "word.h"

namespace linguistic_processor {
namespace morphological_analysis {

std::ostream& operator << (std::ostream& out, const Word& w) {
    out << w.word << " (";
    for (size_t i = 0; i < w.grammeme.size(); ++i) {
        if (i != 0) {
            out << ", ";
        }
        out << w.grammeme[i].description;
    }
    out << ")";
    return out;
}

std::ostream& operator << (std::ostream& out, const Lemma& l) {
    out << "id: " << l.id << '\n';
    out << "words: " << '\n';
    for (size_t i = 0; i < l.words.size(); ++i) {
        out << "    " << l.words[i];
        if (i + 1 != l.words.size()) {
            out << '\n';
        }
    }
    return out;
}

} // namespace morphological_analysis
} // namespace linguistic_processor
