#!/bin/sh

CFILE="src/version.hpp"
REV=`git rev-parse --short HEAD`
RCOUNT=`git shortlog -s | awk '{ SUM += $1 } END { print SUM }'`
VER="$REV-$RCOUNT"

echo "#ifndef VERSION_HPP
#define VERSION_HPP

string version = \"$VER\";

#endif // VERSION_HPP
// vim:ts=4 et sw=4 sts=4" > $CFILE
