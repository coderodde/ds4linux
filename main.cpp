#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <linux/limits.h>
#include <pwd.h>
#include <unistd.h>
#include <vector>

const std::string RELATIVE_TAG_FILE_PATH = "/.dt/tags";

static const std::string getTagFilePath() {
    char* c_home_directory = getpwuid(getuid())->pw_dir;
    size_t str_len = strlen(c_home_directory) + 1;
    char* c_home_directory_copy = (char*) std::malloc(str_len);
    std::strncpy(c_home_directory_copy,
                 c_home_directory,
                 str_len);

    std::string path(c_home_directory_copy);
    return path += RELATIVE_TAG_FILE_PATH;
}

static const std::string getCurrentWorkingDirectory() {
    char* working_dir = (char*) std::malloc(PATH_MAX);
    working_dir = getcwd(working_dir, PATH_MAX);
    return std::string(working_dir);
}

static size_t computeEditDistance(const std::string& str1,
                                  const std::string& str2) {
    std::vector<std::vector<char>> dist_matrix();



    return 0;
}

const std::string OPERATION_DESCRIPTOR_MESSAGE = "message";
const std::string OPERATION_SWITCH_DIRECTORY = "switch_directory";
const std::string OPERATION_DESCRIPTOR_SHOW_TAG_ENTRY_LIST =
        "show_tag_entry_list";

const std::string FLAG_UPDATE_PREVIOUS = "--update-previous";
const std::string FLAG_LIST_TAGS = "-l";
const std::string FLAG_LIST_BOTH = "-L";
const std::string FLAG_LIST_TAGS_SORTED = "-s";
const std::string FLAG_LIST_BOTH_SORTED = "-S";
const std::string FLAG_LIST_BOTH_SORTED_DIRS = "-d";
const std::string PREV_TAG_NAME = "__dt_previous";

using com::github::coderodde::dtpp4linux::DirectoryTagEntry;
using com::github::coderodde::dtpp4linux::DirectoryTagEntryList;

static void updatePreviousDirectory(
        DirectoryTagEntryList& directoryTagEntryList,
        const std::string& newDirectoryName) {

}

int main() {

    using std::cout;
    using com::github::coderodde::dtpp4linux::DirectoryTagEntry;

    cout << getTagFilePath() << "\n";
    DirectoryTagEntry e("bin", "/usr/local/bin");


    cout << e.getLevenshteinDistance("xdind") << "\n";

    return 0;
}
