//// ////////////////////////////////////////////// ////
 // Created by Rodion "rodde" Efremov on 19.11.2020. //
//// ////////////////////////////////////////////// ////

#include "DirectoryTagEntry.h"
#include <utility>
#include <vector>

namespace com::github::coderodde::ds4mac {

    using std::size_t;

    DirectoryTagEntry::DirectoryTagEntry(
        std::string tagName_,
        std::string directoryName_) :
        tagName{std::move(tagName_)},
        directoryName{std::move(directoryName_)} {}

    const std::string& DirectoryTagEntry::getTagName() const {
        return tagName;
    }

    const std::string& DirectoryTagEntry::getDirectoryName() const {
        return directoryName;
    }      

    void DirectoryTagEntry::setTagName(const std::string& newTagName) {
        tagName = newTagName;
    }

    void DirectoryTagEntry::setDirectoryName(const std::string& newDirectoryName) {
        directoryName = newDirectoryName;
    }

    size_t DirectoryTagEntry::getLevenshteinDistance(const std::string str) const {
        const size_t len1 = str.length() + 1;
        const size_t len2 = tagName.length() + 1;

        std::vector<std::vector<size_t>> distanceMatrix(len1);

        for (size_t i = 0; i < len1; i++) {
            std::vector<size_t> row(len2);
            distanceMatrix[i] = row;
        }

        for (size_t i = 1; i < len1; i++) {
            distanceMatrix[i][0] = i;
        }

        for (size_t i = 1; i < len2; i++) {
            distanceMatrix[0][i] = i;
        }

        for (size_t i1 = 1; i1 < len1; i1++) {
            for (size_t i2 = 1; i2 < len2; i2++) {
                size_t cost = (str[i1 - 1] == tagName[i2 - 1] ? 0 : 1);

                distanceMatrix[i1][i2] =
                       std::min(
                                std::min(distanceMatrix[i1 - 1][i2] + 1,
                                         distanceMatrix[i1][i2 - 1] + 1),
                                         distanceMatrix[i1 - 1][i2 - 1] + cost);
            }
        }      

        return distanceMatrix[len1 - 1][len2 - 1];
    }
}