#!/bin/bash

dir=`git rev-parse --show-toplevel`

if [[ $# != 1 ]]; then
	echo "Usage: $0 <version>"
	exit 1
fi

version=$1

echo "Changing version to $version ... "

# /scripts/generate_single_header.sh
sed -i "s/Version [^0-9.]*\([0-9.]*\).*/Version $version/" \
		"$dir/scripts/generate_single_header.sh" || exit 2 #\
		#"<other files>" \

# /CMakeLists.txt
sed -i "s/ VERSION [^0-9.]*\([0-9.]*\).*/ VERSION $version)/" \
		"$dir/CMakeLists.txt" || exit 3

echo "Done. Generation of CHANGELOG.md ..."
echo ""

cd ${dir}
github_changelog_generator terae/graph
git add CHANGELOG.md
