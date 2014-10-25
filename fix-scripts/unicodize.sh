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
  echo $f
  echo $fname

# Combine into original file
  $sedcmd -e 's/\xC4/\xC3\x84/g' < $f |\
  $sedcmd -e 's/\xC5/\xC3\x85/g' |\
  $sedcmd -e 's/\xD6/\xC3\x96/g' |\
  $sedcmd -e 's/\xE4/\xC3\xA4/g' |\
  $sedcmd -e 's/\xE5/\xC3\xA5/g' |\
  $sedcmd -e 's/\xF6/\xC3\xB6/g' |\
  $sedcmd -e 's/\xE9/\xC3\xA9/g' |\
  $sedcmd -e 's/\xC9/\xC3\x89/g' > $f.new
  mv $f.new $f
done





