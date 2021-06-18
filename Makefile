all: DirectoryTagEntry.cpp DirectoryTagEntryList.cpp main.cpp
	g++ -std=c++17 -Wall -o dt_engine DirectoryTagEntry.cpp DirectoryTagEntryList.cpp main.cpp

install:
	chmod 500 installer.sh
	./installer.sh
