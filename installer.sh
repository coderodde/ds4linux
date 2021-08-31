#!/usr/bin/env bash

script_magic="alias ds='source ~/.ds/ds_script'"

echo "Installing ds..."

grep "$script_magic" ~/.bashrc 

if [ $? != 0 ]; then
	echo "$script_magic" >> ~/.bashrc	
	echo "~/.bashrc updated!"
else
	echo "~/.bashrc is already updated."
fi

# Create files:
echo "Creating files..."
mkdir -p ~/.ds
echo "Created the .ds directory."
make > /dev/null
cp ds_engine ~/.ds/ds_engine
echo "Built the ds_engine."
tag_file=~/.ds/tags
touch tag_file

add_tag_to_file () {
	grep $1 $tag_file > /dev/null
	if [ $? != 0 ]; then
		echo $1 $2 >> $tag_file
		echo Setting the tag $1 to directory $2 done.
	fi
}

# Populate the default 
echo "Populating the tag file with default tags..."

add_tag_to_file "docs" "~/Documents"
add_tag_to_file "down" "~/Downloads"
add_tag_to_file "root" "/"
add_tag_to_file "home" "~"
add_tag_to_file "ds"   "~/.ds"

echo "Done populating the tag file with default tags."
echo "Copying the script..."

cp ds_script ~/.ds/ds_script

echo "Done! ds will be available for use in your next shell session. :-]"
