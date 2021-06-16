//// ////////////////////////////////////////////// ////
 // Created by Rodion "rodde" Efremov on 20.11.2020. //
//// ////////////////////////////////////////////// ////

#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <limits>

using com::github::coderodde::dtpp4linux::DirectoryTagEntry;

const size_t com::github::coderodde::dtpp4linux
::DirectoryTagEntryList::size() const {
    return entries.size();
}

void com::github::coderodde::dtpp4linux
::DirectoryTagEntryList::appendDirectoryTagEntry(
        const DirectoryTagEntry& dte) {
    entries.push_back(dte);
}

DirectoryTagEntry com::github::coderodde::dtpp4linux
::DirectoryTagEntryList::operator[](size_t index) const {
    return entries[index];
}

DirectoryTagEntry com::github::coderodde::dtpp4linux
::DirectoryTagEntryList::operator[](const std::string& targetDirectoryName) {
    const DirectoryTagEntry* ptrBestDirectoryEntry;
    size_t bestLevenshteinDistance = std::numeric_limits<size_t>::max();

    for (const DirectoryTagEntry const& dte : entries) {
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
