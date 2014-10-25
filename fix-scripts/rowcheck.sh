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
  # Create file containing EOL information
  $sedcmd -z -e 's/[^\x0D\x0A]//g' $endir$fname > $fname.orignl

  # Create file containing EOL information
  $sedcmd -z -e 's/[^\x0D\x0A]//g' $svdir$fname > $fname.newnl

  diff $fname.newnl $fname.orignl

# Remove temporary eol-file and nocr-file
  rm $fname.orignl
  rm $fname.newnl
done





