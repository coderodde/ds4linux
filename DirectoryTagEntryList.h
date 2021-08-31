//// /////////////////////////////////////////////  ////
 // Created by Rodion "rodde" Efremov on 20.11.2020. //
//// ////////////////////////////////////////////// ////

#ifndef COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_LIST_H
#define COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_LIST_H

#include "DirectoryTagEntry.h"
#include <vector>

namespace com::github::coderodde::ds4mac {

    class DirectoryTagEntryList {
    private:
        std::vector<DirectoryTagEntry> entries;

    public:
        const size_t size() const;
        DirectoryTagEntryList& operator<<(DirectoryTagEntry const& directoryTagEntry);
        DirectoryTagEntry at(size_t index) const;
        DirectoryTagEntry* operator[](std::string const& targetDirectoryName);
        void operator<<(std::ifstream& ifs);
        void sortByTags();
        void sortByDirectories();
        void listTags();
        void listTagsAndDirectories();
        friend void operator>>(std::ifstream&, DirectoryTagEntryList&);
        friend void operator>>(DirectoryTagEntryList const&, 
                               std::ofstream&);
    };
}

#endif // COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_LIST_H
