#!/bin/bash

# Create file containing EOL information
gsed -z -e 's/[^\x0D\x0A]//g' en/$1 > $1.eol

# Remove all CR from infile, clean unicode damage as well
gsed -z -e 's/\x0D//g' $1 > $1.nocr

# Combine into original file
paste -d '\n' $1.nocr $1.eol |\
  gsed -e 's/\xC3\x84/\xC4/g' |\
  gsed -e 's/\xC3\x85/\xC5/g' |\
  gsed -e 's/\xC3\x96/\xD6/g' |\
  gsed -e 's/\xC3\xA4/\xE4/g' |\
  gsed -e 's/\xC3\xA5/\xE5/g' |\
  gsed -e 's/\xC3\xB6/\xF6/g' |\
  gsed -e 's/\xC3\xA9/\xE9/g' |\
  gsed -e 's/\xC3\x89/\xC9/g' |\
  gsed -z -e 's/\n\(\r*\n\)/\1/g' > $1

# Clean
rm $1.eol
rm $1.nocr


