#!/bin/bash

HOME=`git rev-parse --show-toplevel`

echo -n "Formating the code with Artistic Style ... "
astyle --style=google --indent=spaces=4 --indent-modifiers --indent-switches --indent-preproc-block --indent-preproc-define --indent-namespaces --indent-col1-comments --pad-oper --pad-comma --pad-header --align-pointer=type --align-reference=name --add-brackets --convert-tabs --close-templates --lineend=linux --preserve-date --suffix=none --formatted ${HOME}/src/* ${HOME}/test/src/unit-*.cpp ${HOME}/examples/*.cpp ${HOME}/benchmarks/*/*.cpp > /tmp/out.log 2>&1 || exit 1
echo "Done."

[ -s /tmp/out.log ]
if [ $? -eq 0 ]; then
	echo -e "\033[0;34m"
	cat /tmp/out.log
	echo ""
	echo -e "\033[0;41mPlease commit (or not) these changes and run the script again.\033[0;0m"
	echo ""
	exit 1
fi

echo -n "Generating the header-only file        ... "

LOCATION=${HOME}/single_include
mkdir -p "$LOCATION"
FILE="$LOCATION/graph.hpp"

cat > "$FILE" << EOF
/*
 *    ╔═╗╦═╗╔═╗╔═╗╦ ╦  C++ Graph library
 *    ║ ╦╠╦╝╠═╣╠═╝╠═╣  Version 1.0.1
 *    ╚═╝╩╚═╩ ╩╩  ╩ ╩  https://github.com/Terae/Structure
 *
 *
 * Single header-file generated by $USER on $(date +%F)
 *
 *
 * Licensed under the MIT License <https://opensource.org/licenses/MIT>.
 * Copyright (c) 2017 Benjamin BIGEY
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to wholm the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 *
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

EOF

# Copying Graph.h, Graph.cpp and Node.cpp into graph.hpp
sed -e '/#include "Graph.cpp"/ {' -e "r ${HOME}/src/Node.cpp" -e "r ${HOME}/src/Graph.cpp" -e 'd' -e '}' ${HOME}/src/Graph.h >> "$FILE" || exit 2
# Copying Node.h into graph.hpp
sed -i -e '/#include "Node.h"/ {' -e "r ${HOME}/src/Node.h" -e 'd' -e '}' "$FILE" || exit 2
sed -i '/#include "Node.cpp"/,+2d' "$FILE" || exit 2
sed -i 's/#include <Node.h>//' "$FILE" || exit 2
sed -i 's/#include <Graph.h>//' "$FILE" || exit 2
# Copying exception.hpp into graph.hpp
sed -i -e '/#include "detail.hpp"/ {' -e "r ${HOME}/src/detail.hpp" -e 'd' -e '}' "$FILE" || exit 2
# Modifying #ifndef/#define values
sed -i 's/ROOT_GRAPH_H/ROOT_GRAPH_FINAL_H/' "$FILE" || exit 2
sed -i '/ROOT_NODE_H/d' "$FILE" || exit 2
sed -i '/ROOT_DETAIL_H/d' "$FILE" || exit 2
# Remove comments
sed -i 's/\/\/\/.*//' "$FILE" || exit 2
# Remove trailing whitespaces
sed -i 's/[ \t]*$//' "$FILE" || exit 2
# Remove unnecessary EOF
sed -i '/^$/N;/\n$/D' "$FILE" || exit 2

git add ${HOME}/single_include/graph.hpp

echo "Done."
