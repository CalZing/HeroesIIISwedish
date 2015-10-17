#!/bin/bash

# Cleaning script for H3 Swedish. Adds/Removes necessary linebreaks
# and restores Unicode damage from certain editors

sedcmd=sed
# use gsed if available (OS X)
which gsed > /dev/null && sedcmd=gsed


grepstring="."
[ $1 ] && grepstring=$1
svdir=../../text/
[ $2 ] && svdir=$2
endir=../../text/original/
[ $3 ] && endir=$3

corrfiles=$(grep -l $grepstring $endir*.txt $endir*.TXT)

for f in $corrfiles
do
  fname=$($sedcmd -e 's&.*\/&&g' <<< $f)
  [ -e $svdir$fname ] || continue
  echo "$fname? (y/n/b)"
  read -n 1 check
  [ $check != "y" ] || {
    vim -O2 $endir$fname $svdir$fname
  }
  [ $check != "b" ] || {
    break
  }
  echo ""

  # Create file containing EOL information
done





