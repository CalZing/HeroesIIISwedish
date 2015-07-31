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
hdreldir=../../release/_HD3_Data/Lang/
[ $3 ] && hdreldir=$4

hdfile=\#sv.ini
for f in $svdir*.txt $svdir*.TXT $svdir*.ini
do
  fname=$($sedcmd -e 's&.*\/&&g' <<< $f)
  echo $fname

  ./txtfix -i $svdir$fname -o $reldir$fname -l $endir$fname -t latin1 -b
done

mv "$reldir"en.ini "$hdreldir"\#sv.ini
