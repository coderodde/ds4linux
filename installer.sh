#!/usr/bin/bash

script_magc="alias dt='source ~/.dt/dt_script"

echo "Installing dt..."

grep $script_magc ~/.bashrc 

if [ $? != 1 ]; then
	echo "alias dt='source ~/.dt/dt_script" >> ~/.bashrc	
	echo "~/.bashrc updated!"
else
	echo "~/.bashrc is already updated."
fi

# Create files:
echo "Creating files..."
mkdir "~/.dt"
make
cp dt_engine ~/.dt/dt_engine
tag_file="~/.dt/tags"
touch $tag_file

add_tag_to_file(tag, dir) {
	if [ grep $tag $tag_file != 0 ]; then
		echo "$tag $dir"  >> $tag_file
		echo "Setting the tag \"$tag\" to directory \"$dir\""
	fi
}

# Populate the default 
echo "Populating the tag file with default tags..."
add_tag_to_file("docs", "~/Documents")
add_tag_to_file("down", "~/Downloads")
add_tag_to_file("root", "/")
add_tag_to_file("home", "~")
echo "Done populating the tag file with default tags."

echo "Done! dt will be available for use in your next shell session. :-]"