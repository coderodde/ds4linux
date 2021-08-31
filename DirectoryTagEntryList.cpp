//// ////////////////////////////////////////////// ////
 // Created by Rodion "rodde" Efremov on 20.11.2020. //
//// ////////////////////////////////////////////// ////

#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <algorithm>
#include <limits>
#include <string>
//#include <linux/limits.h>

using com::github::coderodde::ds4mac::DirectoryTagEntry;
using std::getline;
using std::string;

namespace com::github::coderodde::ds4mac {

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

    DirectoryTagEntry* DirectoryTagEntryList::operator[](
        const std::string& targetDirectoryName) {
    
        DirectoryTagEntry* ptrBestDirectoryEntry;
        size_t bestLevenshteinDistance = std::numeric_limits<size_t>::max();

        for (DirectoryTagEntry& dte : entries) {
            size_t levenshteinDistance = dte.getLevenshteinDistance(targetDirectoryName);

            if (levenshteinDistance == 0) {
                return &dte;
            }

            if (bestLevenshteinDistance > levenshteinDistance) {
                bestLevenshteinDistance = levenshteinDistance;
                ptrBestDirectoryEntry = &dte;
            }
        }

        return ptrBestDirectoryEntry;
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

    // trim from start (in place)
    static inline void ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string &s) {
        ltrim(s);
        rtrim(s);
    }

    void operator>>(
        std::ifstream& ifs,
        DirectoryTagEntryList& directoryTagEntryList) {

        while (ifs.good() && !ifs.eof()) {

            string tag;
            ifs >> tag;
            trim(tag);

            // Grab the rest of the line. 
            // We need this isntead of >> in order to obtain 
            // the space characters in the directory names.
            string dir;
            getline(ifs, dir);
            trim(dir);

            DirectoryTagEntry newDirectoryEntry(tag, dir);
            directoryTagEntryList << newDirectoryEntry;
        }
    }

    void operator>>(DirectoryTagEntryList const& directoryTagEntryList,
                    std::ofstream& ofs) {
        for (size_t i = 0, sz = directoryTagEntryList.size();
            i < sz;
            i++) {
            DirectoryTagEntry const& dte = directoryTagEntryList.at(i);
            ofs << dte.getTagName() << " " << dte.getDirectoryName() << "\n";
        }
    }
}