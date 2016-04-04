#!/bin/bash

# This script is used for compile to binary files
# and add version information
#
# Usage: cd scripts && bash ./compile_to_bin.sh


UNAMES="$(uname -s)"

if [[ $UNAMES == 'Linux' ]]
then
    OS='linux'
elif [[ $UNAMES == 'Darwin' ]]
then
    OS='macos'
elif [[ $UNAMES == CYGWIN* ]] || [[ $UNAMES == MINGW* ]]
then
    OS='windows'
else
    OS='unknown'
fi

printf "You are on the %s platform (%s)\n\n" "$OS" "$UNAMES"

PROGRAM_NAME=../bin/nodefinderc-$OS

echo 'Start compling...'
echo ../nodefinderc.c -o $PROGRAM_NAME
gcc ../nodefinderc.c -o $PROGRAM_NAME

PROGRAM_RAW_VERSION="$($PROGRAM_NAME -v)"
PROGRAM_VERSION="${PROGRAM_RAW_VERSION##* }"

echo
echo 'Add Version information...'
mv ../bin/$PROGRAM_NAME ../bin/$PROGRAM_NAME-$PROGRAM_VERSION
echo 'mv ' $PROGRAM_NAME $PROGRAM_NAME-$PROGRAM_VERSION

echo
echo 'Done!'

