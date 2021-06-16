//// ////////////////////////////////////////////// ////
 // Created by Rodion "rodde" Efremov on 19.11.2020. //
//// ////////////////////////////////////////////// ////

#include "DirectoryTagEntry.h"
#include <utility>
#include <vector>

com::github::coderodde::dtpp4linux::DirectoryTagEntry::DirectoryTagEntry(
        std::string tagName_,
        std::string directoryName_) :
        tagName{std::move(tagName_)},
        directoryName{std::move(directoryName_)} {}

const std::string& com::github::coderodde::dtpp4linux::DirectoryTagEntry
::getTagName() const {
    return tagName;
}

const std::string& com::github::coderodde::dtpp4linux::DirectoryTagEntry
::getDirectoryName() const {
    return directoryName;
}

void com::github::coderodde::dtpp4linux::DirectoryTagEntry
::setDirectoryName(const std::string& newDirectoryName) {
    this->directoryName = newDirectoryName;
}

void com::github::coderodde::dtpp4linux::DirectoryTagEntry
::setTagName(const std::string& newTagName) {
    this->tagName = newTagName;
}

size_t com::github::coderodde::dtpp4linux::
DirectoryTagEntry::getLevenshteinDistance(const std::string str) const {
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

    for (int i1 = 1; i1 < len1; i1++) {
        for (int i2 = 1; i2 < len2; i2++) {
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