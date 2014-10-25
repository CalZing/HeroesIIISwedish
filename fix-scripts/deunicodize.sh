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
  cat $f |\
  $sedcmd -e 's/\xC3\x84/\xC4/g' |\
  $sedcmd -e 's/\xC3\x85/\xC5/g' |\
  $sedcmd -e 's/\xC3\x96/\xD6/g' |\
  $sedcmd -e 's/\xC3\xA4/\xE4/g' |\
  $sedcmd -e 's/\xC3\xA5/\xE5/g' |\
  $sedcmd -e 's/\xC3\xB6/\xF6/g' |\
  $sedcmd -e 's/\xC3\xA9/\xE9/g' |\
  $sedcmd -e 's/\xC3\x89/\xC9/g' |\
  $sedcmd -z -e 's/\n\(\r*\n\)/\1/g' > $f.new
  mv $f.new $f
done





