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
using std::cerr;
using std::cout;

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

/*static const size_t getMaximumTagLength(DirectoryTagEntryList const& dtel) {
    size_t maximumTagLength = 0;

    for (size_t index = 0, sz = dtel.size(); index < sz; index++) {
        maximumTagLength = 
            std::max(maximumTagLength,
                     dtel.at(index).getTagName().length());
    }

    return maximumTagLength;
}*/

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

static int updatePreviousDirectory(
        const std::string& newDirectoryName) {

    std::string tagFilePath = getTagFilePath();
    std::ifstream ifs;
    ifs.open(tagFilePath, std::ifstream::in);

    DirectoryTagEntryList directoryTagEntryList;
    ifs >> directoryTagEntryList;

    std::cout << "updatePreviousDirectory: " << directoryTagEntryList.size() << '\n';

    return EXIT_SUCCESS;
}

static int switchDirectory(std::string const& tag) {
    DirectoryTagEntryList directoryTagEntryList;
    std::ifstream ifs;
    std::string tagFilePath = getTagFilePath();
    ifs.open(tagFilePath, std::ifstream::in);

    if (!ifs.good()) {
        cerr << "Error: could not open the tag file \"" 
             << tagFilePath
             << "\"\n";
    }

    ifs >> directoryTagEntryList;

    if (directoryTagEntryList.size() == 0) {
        cout << OPERATION_DESCRIPTOR_MESSAGE 
             << "\n"
             << "Warning: the tag file is empty.";

        return EXIT_SUCCESS;
    }

    if (directoryTagEntryList.size() == 1) {
        cout << OPERATION_SWITCH_DIRECTORY
             << "\n"
             << directoryTagEntryList[0].getDirectoryName();

        updatePreviousDirectory(getCurrentWorkingDirectory());

        std::ofstream ofs;
        ofs.open(getTagFilePath(), std::ofstream::out);

        directoryTagEntryList >> ofs;

        return EXIT_SUCCESS;
    }

    DirectoryTagEntry bestMatch = directoryTagEntryList[tag];

    // New line?
    cout << OPERATION_SWITCH_DIRECTORY
         << bestMatch.getDirectoryName();

    return EXIT_SUCCESS;
}

static int processSingleFlag(std::string const& flag) {
    std::cout << "flag = " << flag << '\n';

    switchDirectory(flag);

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {

    using std::cerr;
    using std::cout;
    using std::string;
    using com::github::coderodde::dtpp4linux::DirectoryTagEntry;
    using com::github::coderodde::dtpp4linux::DirectoryTagEntryList;
    using std::ifstream;

    if (argc == 1) {
        //jumpToPreviousDirectory();
    } else if (argc == 2) {
        processSingleFlag(argv[1]);
    } else if (argc == 3) {
        updatePreviousDirectory(argv[2]);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
/*
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
    return EXIT_SUCCESS;*/
}
