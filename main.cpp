#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <linux/limits.h>
#include <pwd.h>
#include <stdexcept>
#include <unistd.h>
#include <vector>

using com::github::coderodde::dtpp4linux::DirectoryTagEntry;
using com::github::coderodde::dtpp4linux::DirectoryTagEntryList;
using std::cerr;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::setw;
using std::string;

//// ///////////////////
 // Operation names: //
/////////////////// ////
const string OPERATION_DESCRIPTOR_MESSAGE = "message";
const string OPERATION_SWITCH_DIRECTORY = "switch_directory";
const string OPERATION_DESCRIPTOR_SHOW_TAG_ENTRY_LIST =
        "show_tag_entry_list";

//// /////////////////
 // All the flags: //
///////////////// ////
const string FLAG_UPDATE_PREVIOUS = "--update-previous";
const string FLAG_LIST_TAGS = "-l";
const string FLAG_LIST_BOTH = "-L";
const string FLAG_LIST_TAGS_SORTED = "-s";
const string FLAG_LIST_BOTH_SORTED = "-S";
const string FLAG_LIST_BOTH_SORTED_DIRS = "-d";
const string PREV_TAG_NAME = "__dt_previous";

// The path to the tag file, relative to the home directory:
const string RELATIVE_TAG_FILE_PATH = "/.dt/tags";

//// ///////////////////////////////////////////
 // Returns the entire path to the tag file. //
/////////////////////////////////////////// ////
static const string getTagFilePath() {
    char* c_home_directory = getpwuid(getuid())->pw_dir;
    size_t str_len = strlen(c_home_directory) + 1;
    char* c_home_directory_copy = (char*) std::malloc(str_len);
    std::strncpy(c_home_directory_copy,
                 c_home_directory,
                 str_len);

    string path(c_home_directory_copy);
    return path += RELATIVE_TAG_FILE_PATH;
}

//// /////////////////////////////////////////
 // Returns the current working directory: //
///////////////////////////////////////// ////
static string getCurrentWorkingDirectory() {
    char* working_dir = new char[PATH_MAX];
    working_dir = getcwd(working_dir, PATH_MAX);
    string rv = working_dir;
    delete[] working_dir;
    return rv;
}

//// //////////////////////////////////////
 // Returns the maximum length of tags: //
////////////////////////////////////// ////
static const size_t getMaximumTagLength(DirectoryTagEntryList const& dtel) {
    size_t maximumTagLength = 0;

    for (size_t index = 0, sz = dtel.size(); index < sz; index++) {
        maximumTagLength = 
            std::max(maximumTagLength,
                     dtel.at(index).getTagName().length());
    }

    return maximumTagLength;
}

//// //////////////////////////////////
 // Updates the previous directory: //
////////////////////////////////// ////
static string updatePreviousDirectory(
        DirectoryTagEntryList& directoryTagEntryList,
        string& newDirectoryName) {

    DirectoryTagEntry dte = directoryTagEntryList[PREV_TAG_NAME];

    if (dte.getTagName() == PREV_TAG_NAME) {
        string rv = dte.getDirectoryName();
        dte.setDirectoryName(newDirectoryName);
        return rv;
    } else {
        DirectoryTagEntry ndte(PREV_TAG_NAME, newDirectoryName);
        directoryTagEntryList << ndte;
        return getCurrentWorkingDirectory();
    }
}

//// //////////////////////////////////////////////////////////////////
 // Jumps to the directory to which dt was switching most recently: //
////////////////////////////////////////////////////////////////// ////
static int jumpToPreviousDirectory() {
    string tagFilePath = getTagFilePath();
    ifstream ifs;
    ifs.open(tagFilePath, std::ifstream::in);

    if (!ifs.is_open() || !ifs.good()) {
        return EXIT_FAILURE;
    }

    DirectoryTagEntryList directoryTagEntryList;
    ifs >> directoryTagEntryList;


    DirectoryTagEntry directoryTagEntry = directoryTagEntryList[PREV_TAG_NAME];
    string nextPath;
    string currentWorkingDirectory = getCurrentWorkingDirectory();

    if (directoryTagEntry.getTagName() == PREV_TAG_NAME) {
        nextPath = directoryTagEntry.getDirectoryName();
        updatePreviousDirectory(directoryTagEntryList, 
                                currentWorkingDirectory);
    } else {
        DirectoryTagEntry prevTagEntry(PREV_TAG_NAME,
                                       currentWorkingDirectory);

        directoryTagEntryList << prevTagEntry;
        nextPath = currentWorkingDirectory;
    }

    ofstream ofs;
    ofs.open(tagFilePath, ofstream::out);

    if (!ofs.is_open() || !ofs.good()) {
        return EXIT_FAILURE;
    }

    directoryTagEntryList >> ofs;
    ofs.close();

    cout << OPERATION_SWITCH_DIRECTORY
         << '\n'
         << nextPath;

    return EXIT_SUCCESS;
}

//// /////////////////////////////
 // Switches to the directory: //
///////////////////////////// ////
static int switchDirectory(std::string const& tag) {
    DirectoryTagEntryList directoryTagEntryList;
    std::ifstream ifs;
    std::string tagFilePath = getTagFilePath();
    ifs.open(tagFilePath, std::ifstream::in);

    if (!ifs.good()) {
        cout << OPERATION_DESCRIPTOR_MESSAGE
             << "\nError: could not open the tag file \""
             << tagFilePath
             << "\"";

        return EXIT_FAILURE;
    }

    ifs >> directoryTagEntryList;

    string currentWorkingDirectory = getCurrentWorkingDirectory();

    if (directoryTagEntryList.size() == 0) {
        string nextDirectory = 
            updatePreviousDirectory(
                directoryTagEntryList,
                currentWorkingDirectory);

        std::ofstream ofs;
        ofs.open(getTagFilePath(), std::ofstream::out);

        if (!ofs.is_open() || !ofs.good()) {
            return EXIT_FAILURE;
        }

        directoryTagEntryList >> ofs;
        ofs.close();

        cout << OPERATION_SWITCH_DIRECTORY
             << '\n'
             << nextDirectory;

        return EXIT_SUCCESS;
    }

    if (directoryTagEntryList.size() == 1) {
        cout << OPERATION_SWITCH_DIRECTORY
             << "\n"
             << directoryTagEntryList[0].getDirectoryName();

        updatePreviousDirectory(directoryTagEntryList,
                                currentWorkingDirectory)  ;

        std::ofstream ofs;
        ofs.open(getTagFilePath(), std::ofstream::out);

        if (!ofs.is_open() || !ofs.good()) {
            return EXIT_FAILURE;
        }

        directoryTagEntryList >> ofs;
        ofs.close();
        return EXIT_SUCCESS;
    }

    DirectoryTagEntry bestMatch = directoryTagEntryList[tag];

    std::ofstream ofs;
    ofs.open(getTagFilePath(), std::ofstream::out);

    if (!ofs.is_open() || !ofs.good()) {
        return EXIT_FAILURE;
    }

    updatePreviousDirectory(directoryTagEntryList, 
                            currentWorkingDirectory);

    directoryTagEntryList >> ofs;
    ofs.close();

    // New line?
    cout << OPERATION_SWITCH_DIRECTORY
         << '\n'
         << bestMatch.getDirectoryName();

    return EXIT_SUCCESS;
}

//// //////////////////////////////////////
 // Lists taga without the directories: //
////////////////////////////////////// ////
static void listTagsOnly(
    DirectoryTagEntryList const& directoryTagEntryList) {
    for (size_t index = 0, sz = directoryTagEntryList.size();
         index < sz;
         index++) {
        std::cout << 
            directoryTagEntryList.at(index).getTagName()
            << '\n';
    }
}

//// //////////////////////////////
 // Lists taga and directories: //
////////////////////////////// ////
static void listTagsAndDirectories(
        DirectoryTagEntryList const& directoryTagEntryList) {
    size_t maxTagLength = getMaximumTagLength(directoryTagEntryList);

    for (size_t index = 0, sz = directoryTagEntryList.size(); 
         index < sz; 
         index++) {
        DirectoryTagEntry const& directoryTagEntry =
            directoryTagEntryList.at(index);

        std::cout << std::setw(maxTagLength + 1)
                  << directoryTagEntry.getTagName() 
                  << ' '
                  << directoryTagEntry.getDirectoryName()
                  << '\n';
    }
}

//// /////////////////////////////////
 // Decided how to print the tags: //
///////////////////////////////// ////
static void listTags(std::string const& flag) {
    std::ifstream ifs;
    ifs.open(getTagFilePath(), std::ifstream::in);

    DirectoryTagEntryList directoryTagEntryList;
    ifs >> directoryTagEntryList;

    if (flag == FLAG_LIST_TAGS_SORTED 
        || flag == FLAG_LIST_BOTH_SORTED) {
        directoryTagEntryList.sortByTags();
    } else if (flag == FLAG_LIST_BOTH_SORTED_DIRS) {
        directoryTagEntryList.sortByDirectories();
    }

    if (flag == FLAG_LIST_TAGS_SORTED
        || flag == FLAG_LIST_TAGS) {
        listTagsOnly(directoryTagEntryList);
    } else if (flag == FLAG_LIST_BOTH 
        || flag == FLAG_LIST_BOTH_SORTED
        || flag == FLAG_LIST_BOTH_SORTED_DIRS) {
        listTagsAndDirectories(directoryTagEntryList);
    } else {
        std::string errorMessage = "Bad flag \"";
        errorMessage += flag;
        errorMessage += "\"";
        std::invalid_argument const& except{errorMessage};
        throw except;
    }
}

//// ///////////////////////////////
 // Processes a single flag/tag: //
/////////////////////////////// ////
static void processSingleFlag(std::string const& flag) {
    if (flag == FLAG_LIST_BOTH
        || flag == FLAG_LIST_TAGS
        || flag == FLAG_LIST_TAGS_SORTED
        || flag == FLAG_LIST_BOTH_SORTED
        || flag == FLAG_LIST_BOTH_SORTED_DIRS) {
        listTags(flag);
    } else {
        switchDirectory(flag);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        jumpToPreviousDirectory();
    } else if (argc == 2) {
        processSingleFlag(argv[1]);
    } else if (argc == 3) {
        string arg{argv[1]};

        if (arg != FLAG_UPDATE_PREVIOUS) {
            return EXIT_FAILURE;
        }

        DirectoryTagEntryList dtel;
        ifstream ifs;
        ifs.open(getTagFilePath(), ifstream::in);

        if (!ifs.is_open()) {
            return EXIT_FAILURE;
        }

        if (!ifs.good()) {
            ifs.close();
            return EXIT_FAILURE;
        }

        ifs >> dtel;
        string newDir{argv[2]};
        updatePreviousDirectory(dtel, newDir);
    } else {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
