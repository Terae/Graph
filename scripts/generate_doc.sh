#!/bin/bash

dir=`git rev-parse --show-toplevel`

cd "$dir/doc"

echo -n "Generation of the API documentation ... "
make doxygen > /dev/null 2>&1
echo "Done."

dest="$dir/../terae.github.io/graph/"
rm -rf "$dest" || exit 1
mkdir -p "$dest"
cp -r "$dir/doc/html/." "$dest" || exit 2

echo ""
read -r -p "Do you want to publish these modifications? [y/N] " response
if [[ $response =~ ^([yY][eE][sS]|[yY])$ ]]
then
	cd "$dir/../terae.github.io"
	git add graph
	read -r -p "Please enter the commit for your changes: " response
	git commit -m "$response"

	read -r -p "Are you sure you want to push? [y/N] " response
	if [[ $response =~ ^([yY][eE][sS]|[yY])$ ]]
	then
		git push
	fi
fi
