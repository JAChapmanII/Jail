#!/bin/sh

CFILE="src/version.hpp"
VER="`git describe`"

echo "#ifndef VERSION_HPP
#define VERSION_HPP

string version = \"$VER\";

#endif // VERSION_HPP
// vim:ts=4 et sw=4 sts=4
" > $CFILE
