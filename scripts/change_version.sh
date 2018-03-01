#!/bin/bash

dir=$(git rev-parse --show-toplevel)

if [[ $# != 1 ]]; then
	echo "Usage: $0 <version>"
	exit 1
fi

version=$1

echo "Changing version to $version ... "

# /scripts/generate_single_header.sh
sed -i "s/Version [^0-9.]*\([0-9.]*\).*/Version $version/" \
		"$dir/scripts/pretty_code.sh" || exit 2 #\
		#"<other files>" \

# /CMakeLists.txt
sed -i "s/ VERSION [^0-9.]*\([0-9.]*\).*/ VERSION $version)/" \
		"$dir/CMakeLists.txt" || exit 3

# /doc/Doxyfile
sed -i "s/PROJECT_NUMBER         = [^0-9.]*\([0-9.]*\).*/PROJECT_NUMBER         = $version/" \
        "$dir/doc/Doxyfile" || exit 4
sed -i "s/@version [^0-9.]*\([0-9.]*\).*/@version $version/" \
		"$dir/doc/index.md" || exit 5

echo "Done. Generation of CHANGELOG.md ..."
echo ""

cd ${dir}

github_changelog_generator terae/graph --simple-list --release-url https://github.com/terae/graph/releases/tag/v${version}
git add CHANGELOG.md
git add CMakeLists.txt
