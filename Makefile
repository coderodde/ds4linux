all: DirectoryTagEntry.cpp DirectoryTagEntryList.cpp main.cpp
	g++ -std=c++17 -Wall -Werror -pedantic -o ds_engine DirectoryTagEntry.cpp DirectoryTagEntryList.cpp main.cpp

install:
	chmod 500 installer.sh
	installer.sh
