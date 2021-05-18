#include "dict.h"

namespace linguistic_processor {
namespace morphological_analysis {

namespace {

std::vector<Lemma> readLemmas(std::istream& inputStream) {
    std::vector<Lemma> lemmas;

    std::string lineBuffer;
    std::vector<std::string> lemmaBuffer;

    while (std::getline(inputStream, lineBuffer)) {
        Lemma lemma;
        lemma.id = std::stoull(lineBuffer);

        while (std::getline(inputStream, lineBuffer)) {
            if (lineBuffer.empty()) {
                break;
            }

            lemmaBuffer.clear();
            for (const char& c : lineBuffer) {
                if (!isspace(c) && c != ',') {
                    if (lemmaBuffer.empty()) {
                        lemmaBuffer.push_back("");
                    }
                    lemmaBuffer.back() += c;
                } else {
                    if (!lemmaBuffer.empty() && !lemmaBuffer.back().empty()) {
                        lemmaBuffer.emplace_back();
                    }
                }
            }

            while (lemmaBuffer.back().empty()) {
                lemmaBuffer.pop_back();
            }

            Word word;
            word.word = lemmaBuffer.at(0);
            for (size_t i = 1; i < lemmaBuffer.size(); ++i) {
                word.grammeme.push_back(FindGrammeme(lemmaBuffer[i]));
            }

            lemma.words.push_back(word);
        }

        lemmas.push_back(lemma);
    }

    return lemmas;
}

std::map<std::string, size_t> buildIndex(const std::vector<Lemma>& lemmas) {
    std::map<std::string, size_t> ret;

    for (size_t i = 0; i < lemmas.size(); ++i) {
        for (const Word& word : lemmas[i].words) {
            if (ret.find(word.word) == ret.end()) {
                ret[word.word] = i;
            }
        }
    }

    return ret;
}

} // namespace

Dict::Dict(std::istream& inputStream) {
    lemmas_ = readLemmas(inputStream);
    index_ = buildIndex(lemmas_);
}

} // namespace morphological_analysis
} // namespace linguistic_processor
