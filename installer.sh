#!/usr/bin/bash

script_magic="alias dt='source ~/.dt/dt_script2"

echo "Installing dt..."

grep "$script_magic" ~/.bashrc 

if [ $? != 0 ]; then
	echo "$script_magic" >> ~/.bashrc	
	echo "~/.bashrc updated!"
else
	echo "~/.bashrc is already updated."
fi

# Create files:
echo "Creating files..."
mkdir -p ~/.dt
echo "Created the .dt directory."
make > /dev/null
cp dt_engine ~/.dt/dt_engine
echo "Built the dt_engine."
tag_file=~/.dt/tags
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

add_tag_to_file "docs", "~/Documents"
add_tag_to_file "down", "~/Downloads"
add_tag_to_file "root", "/"
add_tag_to_file "home", "~"

echo "Done populating the tag file with default tags."
echo "Done! dt will be available for use in your next shell session. :-]"