#!/bin/bash

BINDIR="bin"
DUMP="./$BINDIR/conf_dump"
COMP="./$BINDIR/conf_compile"

DEF="./jailrc.def"
CONF="lib/defines.cpp"
OBJ="./obj/config.o"

if [[ -x $COMP ]]; then
	$COMP $DEF $CONF
	rm -f $COMP $DUMP $OBJ

	echo "make $COMP"
	make $COMP
	if [[ $? != 0 ]]; then
		exit $?
	fi

	echo "make $DUMP"
	make $DUMP
	if [[ $? != 0 ]]; then
		exit $?
	fi
else
	echo > $CONF

	echo "make $COMP"
	make $COMP
	if [[ $? != 0 ]]; then
		exit $?
	fi
	$0
fi

