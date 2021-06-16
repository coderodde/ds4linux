//// /////////////////////////////////////////////  ////
 // Created by Rodion "rodde" Efremov on 20.11.2020. //
//// ////////////////////////////////////////////// ////

#ifndef COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_LIST_H
#define COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_LIST_H

#include "DirectoryTagEntry.h"
#include <vector>

namespace com::github::coderodde::dtpp4linux {

    class DirectoryTagEntryList {
    private:
        std::vector<DirectoryTagEntry> entries;

    public:
        const size_t size() const;
        void appendDirectoryTagEntry(const DirectoryTagEntry& dte);
        DirectoryTagEntry operator[](size_t index) const;
        DirectoryTagEntry operator[](const std::string& targetDirectoryName);
    };
}

#endif // COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_LIST_H
