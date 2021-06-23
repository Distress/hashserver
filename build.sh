#!/bin/bash

PRO_NAME="hashserver"
BUILD_DIR="build"

fail() { echo -e "\033[37;1;41m $1 \033[0m"; exit 1;}

convertsecs() {
     ((h=${1}/3600))
     ((m=(${1}%3600)/60))
     ((s=${1}%60))
     printf "%02d:%02d:%02d\n" $h $m $s
}

START=$(date +%s)

while getopts ":d" opt; do
    case $opt in
        d)
	    $(sudo apt-get -y install )
	    ;;
    esac
done

echo -e "\033[32m Building $PRO_NAME\033[0m"

[ -d $BUILD_DIR ] && { /bin/rm -rf $BUILD_DIR || fail "removing build dir error" ;}

/bin/mkdir $BUILD_DIR || fail "making build dir error"

cd $BUILD_DIR

qmake CONFIG+=build_tests -config release ../ || fail "qmake error"

make || fail "make error"

make check || fail "unit tests error"

make package || fail "package build error"

echo -e "\033[32m $PRO_NAME was builded in $(convertsecs $(($(date +%s)-$START)))\033[0m"

exit 0
