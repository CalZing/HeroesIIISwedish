#!/bin/bash

# Cleaning script for H3 Swedish. Adds/Removes necessary linebreaks
# and restores Unicode damage from certain editors

sedcmd=sed
# use gsed if available (OS X)
which gsed > /dev/null && sedcmd=gsed

svdir=../../release/Data/
[ $1 ] && svdir=$1
endir=../../text/original/
[ $2 ] && endir=$2

for f in $svdir*.txt $svdir*.TXT
do
  fname=$($sedcmd -e 's&.*\/&&g' <<< $f)
  echo $fname

  ./txtfix -i $svdir$fname -l $endir$fname -c
done
