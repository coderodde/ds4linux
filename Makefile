all: DirectoryTagEntry.cpp DirectoryTagEntryList.cpp main.cpp
	g++ -std=c++17 -o dt_engine DirectoryTagEntry.cpp DirectoryTagEntryList.cpp main.cpp

install:
	@echo "installing"
