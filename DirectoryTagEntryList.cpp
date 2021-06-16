//// ////////////////////////////////////////////// ////
 // Created by Rodion "rodde" Efremov on 20.11.2020. //
//// ////////////////////////////////////////////// ////

#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <algorithm>
#include <limits>
#include <string>

using com::github::coderodde::dtpp4linux::DirectoryTagEntry;

const size_t com::github::coderodde::dtpp4linux
::DirectoryTagEntryList::size() const {
    return entries.size();
}

com::github::coderodde::dtpp4linux::DirectoryTagEntryList&
com::github::coderodde::dtpp4linux::DirectoryTagEntryList::operator<<(const DirectoryTagEntry &directoryTagEntry) {
    entries.push_back(directoryTagEntry);
    return *this;
}

DirectoryTagEntry com::github::coderodde::dtpp4linux
::DirectoryTagEntryList::operator[](size_t index) const {
    return entries.at(index);
}

DirectoryTagEntry com::github::coderodde::dtpp4linux
::DirectoryTagEntryList::operator[](const std::string& targetDirectoryName) {
    const DirectoryTagEntry* ptrBestDirectoryEntry;
    size_t bestLevenshteinDistance = std::numeric_limits<size_t>::max();

    for (DirectoryTagEntry const& dte : entries) {
        size_t levenshteinDistance = dte.getLevenshteinDistance(targetDirectoryName);

        if (levenshteinDistance == 0) {
            return dte;
        }

        if (bestLevenshteinDistance > levenshteinDistance) {
            bestLevenshteinDistance = levenshteinDistance;
            ptrBestDirectoryEntry = &dte;
        }
    }

    return *ptrBestDirectoryEntry;
}

void com::github::coderodde::dtpp4linux::DirectoryTagEntryList::operator<<(std::ifstream& ifs) {

    using std::string;

    while (ifs.good()) {
        string tag;
        ifs >> tag;

        char buffer[PATH_MAX];
        ifs.getline(buffer, PATH_MAX);
        string path(buffer);

        DirectoryTagEntry newDirectoryEntry(tag, path);
        entries.push_back(newDirectoryEntry);
    }
}

void com::github::coderodde::dtpp4linux::DirectoryTagEntryList::sortByTags() {
    std::stable_sort( entries.begin(), entries.end(), DirectoryTagEntry::tagComparator);
}

void com::github::coderodde::dtpp4linux::DirectoryTagEntryList::sortByDirectories() {
    std::stable_sort(entries.begin(), entries.end(),  DirectoryTagEntry::directoryComparator);
}