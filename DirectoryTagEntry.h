//// /////////////////////////////////////////////  ////
 // Created by Rodion "rodde" Efremov on 19.11.2020. //
//// ////////////////////////////////////////////// ////
#ifndef COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_H
#define COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_H

#include <fstream>
#include <iostream>
#include <string>

namespace com::github::coderodde::ds4mac {

    class DirectoryTagEntry {
    private:
        std::string tagName;
        std::string directoryName;

    public:

        DirectoryTagEntry(std::string tagName, std::string directoryName);

        const std::string& getTagName() const;
        const std::string& getDirectoryName() const;

        void setTagName(const std::string& newTagName);
        void setDirectoryName(const std::string& newDirectoryName);

        size_t getLevenshteinDistance(std::string str) const;

        friend void operator>>(std::ifstream& ifs, DirectoryTagEntry& dte) {
            std::string readTagName;
            std::string readDirectoryName;

            std::getline(ifs, readDirectoryName);
            dte.setTagName(readTagName);
            dte.setDirectoryName(readDirectoryName);
        }

        friend void operator<<(std::ofstream& ofs, const DirectoryTagEntry& dte) {
            ofs << dte.getTagName() << " " << dte.getDirectoryName();
        }

        static struct {
            bool operator()(DirectoryTagEntry const& a, DirectoryTagEntry const& b) {
                return a.getTagName() < b.getTagName();
            }
        } tagComparator;

        static struct {
            bool operator()(DirectoryTagEntry const& a, DirectoryTagEntry const& b) {
                return a.getDirectoryName() < b.getDirectoryName();
            }
        } directoryComparator;
    };
}

#endif // COM_GITHUB_CODERODDE_DTPP4LINUX_DIRECTORY_TAG_ENTRY_H
