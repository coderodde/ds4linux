#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <linux/limits.h>
#include <pwd.h>
#include <unistd.h>
#include <vector>

using com::github::coderodde::dtpp4linux::DirectoryTagEntryList;

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
    char* working_dir = new char[PATH_MAX];
    working_dir = getcwd(working_dir, PATH_MAX);
    std::string rv = working_dir;
    delete[] working_dir;
    return rv;
}

static const size_t getMaximumTagLength(DirectoryTagEntryList const& dtel) {
    size_t maximumTagLength = 0;

    for (size_t index = 0, sz = dtel.size(); index < sz; index++) {
        maximumTagLength = 
            std::max(maximumTagLength,
                     dtel[index].getTagName().length());
    }

    return maximumTagLength;
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

int main(int argc, char *args[]) {

    using std::cerr;
    using std::cout;
    using std::string;
    using com::github::coderodde::dtpp4linux::DirectoryTagEntry;
    using com::github::coderodde::dtpp4linux::DirectoryTagEntryList;
    using std::ifstream;

    ifstream fs;
    string tagFilePath = getTagFilePath();
    fs.open(tagFilePath);

    if (!fs.is_open()) {
        cerr << "Error: Couuld not open the file \"" << tagFilePath << "\"\n.";
        return EXIT_FAILURE;
    }

    DirectoryTagEntryList directoryTagEntryList;

    directoryTagEntryList << fs;
    cout << directoryTagEntryList.size() << "\n";
    return EXIT_SUCCESS;
}
