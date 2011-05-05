#!/bin/sh

CFILE="src/version.hpp"
VER="`git describe 2>/dev/null`"

if [ -z "$VER" ]; then
    VER="`git rev-list HEAD | wc -l`-`git rev-parse HEAD | cut -c1-8`"
fi

echo "#ifndef VERSION_HPP
#define VERSION_HPP

string version = \"$VER\";

#endif // VERSION_HPP
// vim:ts=4 et sw=4 sts=4
" > $CFILE
