#!/bin/bash

# Cleaning script for H3 Swedish. Adds/Removes necessary linebreaks
# and restores Unicode damage from certain editors

sedcmd=sed
# use gsed if available (OS X)
which gsed > /dev/null && sedcmd=gsed

svdir=../
[ $1 ] && svdir=$1
endir=en/
[ $2 ] && endir=$2

for f in $svdir*.txt $svdir*.TXT
do
  fname=$($sedcmd -e 's&.*\/&&g' <<< $f)
  echo $fname

  # Create file containing EOL information
  ./txtfix -i $svdir$fname -o $svdir$fname -l $endir$fname -t utf8 -b
done





