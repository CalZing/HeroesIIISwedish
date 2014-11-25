#!/bin/bash

# Cleaning script for H3 Swedish. Adds/Removes necessary linebreaks
# and restores Unicode damage from certain editors

sedcmd=sed
# use gsed if available (OS X)
which gsed > /dev/null && sedcmd=gsed

svdir=../../text/
[ $1 ] && svdir=$1
endir=../../text/original/
[ $2 ] && endir=$2
reldir=../../release/Data/
[ $3 ] && reldir=$3

for f in $svdir*.txt $svdir*.TXT
do
  fname=$($sedcmd -e 's&.*\/&&g' <<< $f)
  echo $fname

  # Create file containing EOL information
  ./txtfix -i $svdir$fname -o $reldir$fname -l $endir$fname -t latin1 -b
done





