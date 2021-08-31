#include "DirectoryTagEntry.h"
#include "DirectoryTagEntryList.h"
#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
//#include <linux/limits.h>
#include <pwd.h>
#include <unistd.h>
#include <vector>

using com::github::coderodde::ds4mac::DirectoryTagEntry;
using com::github::coderodde::ds4mac::DirectoryTagEntryList;
using std::cerr;
using std::cout;
using std::ifstream;
using std::ofstream;
using std::setw;
using std::string;

//// ///////////////////
 // Operation names: //
/////////////////// ////
const string OPERATION_SWITCH_DIRECTORY = "switch_directory";
const string OPERATION_DESCRIPTOR_SHOW_TAG_ENTRY_LIST =
        "show_tag_entry_list";

//// /////////////////
 // All the flags: //
///////////////// ////
const string FLAG_LIST_TAGS = "-l";
const string FLAG_LIST_BOTH = "-L";
const string FLAG_LIST_TAGS_SORTED = "-s";
const string FLAG_LIST_BOTH_SORTED = "-S";
const string FLAG_LIST_BOTH_SORTED_DIRS = "-d";
const string FLAG_UPDATE_PREVIOUS = "--update-previous";
const string PREV_TAG_NAME = "__dt_previous";

// The path to the tag file, relative to the home directory:
const string RELATIVE_TAG_FILE_PATH = "/.ds/tags";

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

    DirectoryTagEntry* dte = directoryTagEntryList[PREV_TAG_NAME];

    if (dte->getTagName().compare(PREV_TAG_NAME) == 0) {
        string rv = dte->getDirectoryName();
        dte->setDirectoryName(newDirectoryName);
        return rv;
    } else {
        DirectoryTagEntry ndte(PREV_TAG_NAME, newDirectoryName);
        directoryTagEntryList << ndte;
        return getCurrentWorkingDirectory();
    }
}

//// //////////////////////////////////////////////////
 // Returns the home directory of the current user: //
////////////////////////////////////////////////// ////
static string getHomeDirectory() {
    return string(getpwuid(getuid())->pw_dir);
}

//// //////////////////////////////////////////////////////////////////////
 // Changes the leading tilde to the name of the user's home directory: //
////////////////////////////////////////////////////////////////////// ////
static string convertDirectoryNameToExactDirectoryName(string dir) {
    if (dir.size() == 0) {
        throw "The directory name is empty. This should not happen.";
    }

    if (dir[0] != '~') {
        // Nothing to do.
        return dir;
    }

    string homeDirectory = getHomeDirectory();
    auto iter = dir.cbegin();
    std::advance(iter, 1);

    for (auto it = iter; it != dir.cend(); ++it) {
        homeDirectory.push_back(*it);
    }

    return homeDirectory;
}

static void checkIfstream(ifstream& ifs) {
    if (!ifs.is_open()) {
        throw "Could not open the tag file for reading.";
    }

    if (!ifs.good()) {
        throw "The tag file input stream is not good.";
    }
}

static void checkOfstream(ofstream& ofs) {
    if (!ofs.is_open()) {
        throw "Could not open the tag file for writing.";
    }

    if (!ofs.good()) {
        throw "The tag file output stream is not good.";
    }
}

//// //////////////////////////////////////////////////////////////////
 // Jumps to the directory to which dt was switching most recently: //
////////////////////////////////////////////////////////////////// ////
static void jumpToPreviousDirectory() {
    string tagFilePath = getTagFilePath();
    ifstream ifs;
    ifs.open(tagFilePath, std::ifstream::in);
    checkIfstream(ifs);

    DirectoryTagEntryList directoryTagEntryList;
    ifs >> directoryTagEntryList;

    DirectoryTagEntry* directoryTagEntry = directoryTagEntryList[PREV_TAG_NAME];
    string nextPath;
    string currentWorkingDirectory = getCurrentWorkingDirectory();

    if (directoryTagEntry->getTagName().compare(PREV_TAG_NAME) == 0)  {
        nextPath = updatePreviousDirectory(directoryTagEntryList, 
                                           currentWorkingDirectory);
    } else {
        DirectoryTagEntry prevTagEntry(PREV_TAG_NAME,
                                       currentWorkingDirectory);

        directoryTagEntryList << prevTagEntry;
        nextPath = currentWorkingDirectory;
    }

    ofstream ofs;
    ofs.open(tagFilePath, ofstream::out);
    checkOfstream(ofs);

    directoryTagEntryList >> ofs;
    ofs.close();

    cout << "yeah: " << nextPath << std::endl;

    cout << OPERATION_SWITCH_DIRECTORY
         << '\n'
         << convertDirectoryNameToExactDirectoryName(nextPath)
         << std::flush;
}

//// /////////////////////////////
 // Switches to the directory: //
///////////////////////////// ////
static void switchDirectory(std::string const& tag) {
    DirectoryTagEntryList directoryTagEntryList;
    std::ifstream ifs;
    std::string tagFilePath = getTagFilePath();
    ifs.open(tagFilePath, std::ifstream::in);
    checkIfstream(ifs);

    ifs >> directoryTagEntryList;

    string currentWorkingDirectory = getCurrentWorkingDirectory();

    if (directoryTagEntryList.size() == 0) {
        string nextDirectory = 
            updatePreviousDirectory(
                directoryTagEntryList,
                currentWorkingDirectory);

        std::ofstream ofs;
        ofs.open(getTagFilePath(), std::ofstream::out);
        checkOfstream(ofs);

        directoryTagEntryList >> ofs;
        ofs.close();

        cout << OPERATION_SWITCH_DIRECTORY
             << '\n'
             << convertDirectoryNameToExactDirectoryName(nextDirectory);
    }

    if (directoryTagEntryList.size() == 1) {
        cout << OPERATION_SWITCH_DIRECTORY
             << "\n"
             << directoryTagEntryList[0]->getDirectoryName();

        updatePreviousDirectory(directoryTagEntryList,
                                currentWorkingDirectory)  ;

        std::ofstream ofs;
        ofs.open(getTagFilePath(), std::ofstream::out);
        checkOfstream(ofs);

        directoryTagEntryList >> ofs;
        ofs.close();
    }

    DirectoryTagEntry* bestMatch = directoryTagEntryList[tag];

    std::ofstream ofs;
    ofs.open(getTagFilePath(), std::ofstream::out);
    checkOfstream(ofs);

    updatePreviousDirectory(directoryTagEntryList, 
                            currentWorkingDirectory);

    directoryTagEntryList >> ofs;
    ofs.close();

    // New line?
    cout << OPERATION_SWITCH_DIRECTORY
         << '\n'
         << convertDirectoryNameToExactDirectoryName(
                bestMatch->getDirectoryName());
}

//// //////////////////////////////////////
 // Lists taga without the directories: //
////////////////////////////////////// ////
static void listTagsOnly(
    DirectoryTagEntryList const& directoryTagEntryList) {

    cout << OPERATION_DESCRIPTOR_SHOW_TAG_ENTRY_LIST << '\n';

    for (size_t index = 0, sz = directoryTagEntryList.size();
         index < sz;
         index++) {
        cout << directoryTagEntryList.at(index).getTagName() << "\n";
    }
}

//// //////////////////////////////
 // Lists taga and directories: //
////////////////////////////// ////
static void listTagsAndDirectories(
        DirectoryTagEntryList const& directoryTagEntryList) {
    cout << OPERATION_DESCRIPTOR_SHOW_TAG_ENTRY_LIST << '\n';

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
    checkIfstream(ifs);

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
    }
}

//// ///////////////////////////////
 // Processes a single flag/tag: //
/////////////////////////////// ////
static void processSingleFlag(std::string const& arg) {
    if (arg == FLAG_LIST_BOTH
        || arg == FLAG_LIST_TAGS
        || arg == FLAG_LIST_TAGS_SORTED
        || arg == FLAG_LIST_BOTH_SORTED
        || arg == FLAG_LIST_BOTH_SORTED_DIRS) {
        listTags(arg);
    } else {
        switchDirectory(arg);
    }
}

static void processUpdatePrevious(string& dir) {
    string tagFilePath = getTagFilePath();
    DirectoryTagEntryList directoryTagEntryList;

    ifstream ifs;
    ifs.open(tagFilePath, ifstream::in);
    checkIfstream(ifs);

    ifs >> directoryTagEntryList;
    ifs.close();

    DirectoryTagEntry* previousEntryCandidate = 
        directoryTagEntryList[PREV_TAG_NAME];

    string nextPath;

    if (previousEntryCandidate->getTagName().compare(PREV_TAG_NAME) == 0) {
        nextPath = previousEntryCandidate->getDirectoryName();
        previousEntryCandidate->setDirectoryName(dir);
    } else {
        DirectoryTagEntry prevDirectoryTagEntry(PREV_TAG_NAME, dir);
        nextPath = getCurrentWorkingDirectory();
        directoryTagEntryList << prevDirectoryTagEntry;
    }

    ofstream ofs;
    ofs.open(getTagFilePath(), ofstream::out);
    checkOfstream(ofs);
    directoryTagEntryList >> ofs;
    ofs.close();

    // return nextPath;
}

int main(int argc, char *argv[]) {
    try {
        if (argc == 1) {
            jumpToPreviousDirectory();
        } else if (argc == 2) {
            processSingleFlag(argv[1]);
        } else if (argc == 3) {
            string flag = argv[1];

            if (flag.compare(FLAG_UPDATE_PREVIOUS) != 0) {
                string errorMsg = "Flag ";
                errorMsg = errorMsg.append(flag);
                errorMsg = errorMsg.append(" not recognized.");
                throw errorMsg.data();
            }

            string dir = argv[2];
            processUpdatePrevious(dir);
        }
    } catch (char *const msg) {
        cerr << msg << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
