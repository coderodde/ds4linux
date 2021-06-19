//// ////////////////////////////////////////////// ////
 // Created by Rodion "rodde" Efremov on 20.11.2020. //
//// ////////////////////////////////////////////// ////

#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <algorithm>
#include <limits>
#include <string>
#include <linux/limits.h>

using com::github::coderodde::dtpp4linux::DirectoryTagEntry;

namespace com::github::coderodde::dtpp4linux {

    const size_t DirectoryTagEntryList::size() const {
        return entries.size();
    }

    DirectoryTagEntryList& DirectoryTagEntryList::operator<<(
        const DirectoryTagEntry &directoryTagEntry) {
    
        entries.push_back(directoryTagEntry);
        return *this;
    }

    DirectoryTagEntry DirectoryTagEntryList::at(size_t index) const {
        return entries.at(index);
    }

    DirectoryTagEntry DirectoryTagEntryList::operator[](
        const std::string& targetDirectoryName) {
    
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

    void DirectoryTagEntryList::operator<<(std::ifstream& ifs) {

        using std::string;

    }

    void DirectoryTagEntryList::sortByTags() {
        std::stable_sort(entries.begin(), entries.end(), DirectoryTagEntry::tagComparator);
    }

    void DirectoryTagEntryList::sortByDirectories() {
        std::stable_sort(entries.begin(), entries.end(),  DirectoryTagEntry::directoryComparator);
    }

    void operator>>(
        std::ifstream& ifs,
        DirectoryTagEntryList& directoryTagEntryList) {

        while (ifs.good()) {
            std::string tag;
            ifs >> tag;

            char buffer[PATH_MAX];
            ifs.getline(buffer, PATH_MAX);
            std::string path(buffer);

            DirectoryTagEntry newDirectoryEntry(tag, path);
            directoryTagEntryList << newDirectoryEntry;
        }
    }

    void operator>>(DirectoryTagEntryList const& directoryTagEntryList,
                    std::ofstream& ofs) {
        for (size_t i = 0, sz = directoryTagEntryList.size();
            i < sz;
            i++) {
            DirectoryTagEntry const& dte = directoryTagEntryList.at(i);
            ofs << dte.getTagName() << " " << dte.getDirectoryName();

            if (i < sz - 1) {
                ofs << "\n";
            }
        }
    }
}