//// /////////////////////////////////////////////  ////
 // Created by Rodion "rodde" Efremov on 19.11.2020. //
//// ////////////////////////////////////////////// ////
#ifndef COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_H
#define COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_H

#include <fstream>
#include <iostream>
#include <string>

namespace com::github::coderodde::dtpp4linux {

    class DirectoryTagEntry {
    private:
        std::string tagName;
        std::string directoryName;

    public:

        DirectoryTagEntry(std::string tagName_,
                          std::string directoryName_);

        const std::string& getTagName() const;
        const std::string& getDirectoryName() const;

        void setTagName(const std::string& newTagName);
        void setDirectoryName(const std::string& newDirectoryName);

        size_t getLevenshteinDistance(std::string str) const;

        friend void operator>>(std::ifstream& ifs, DirectoryTagEntry& dte) {
            std::string readTagName;
            std::string readDirectoryName;

            ifs >> readTagName >> readDirectoryName;

            dte.setTagName(readTagName);
            dte.setDirectoryName(readDirectoryName);
        }

        friend void operator<<(std::ofstream& ofs, const DirectoryTagEntry& dte) {
            ofs << dte.getTagName() << " " << dte.getDirectoryName();
        }
    };
}

#endif // COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_H