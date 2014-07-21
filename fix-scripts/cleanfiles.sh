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
  $sedcmd -z -e 's/[^\x0D\x0A]//g' $endir$fname > $fname.eol

  # Remove all CR from translated file, clean unicode damage as well
  $sedcmd -z -e 's/\x0D//g' $svdir$fname > $fname.nocr

# Combine into original file
  paste -d '\n' $fname.nocr $fname.eol |\
  $sedcmd -e 's/\xC3\x84/\xC4/g' |\
  $sedcmd -e 's/\xC3\x85/\xC5/g' |\
  $sedcmd -e 's/\xC3\x96/\xD6/g' |\
  $sedcmd -e 's/\xC3\xA4/\xE4/g' |\
  $sedcmd -e 's/\xC3\xA5/\xE5/g' |\
  $sedcmd -e 's/\xC3\xB6/\xF6/g' |\
  $sedcmd -e 's/\xC3\xA9/\xE9/g' |\
  $sedcmd -e 's/\xC3\x89/\xC9/g' |\
  $sedcmd -z -e 's/\n\(\r*\n\)/\1/g' > $f

# Remove temporary eol-file and nocr-file
  rm $fname.eol
  rm $fname.nocr
done





