#!/bin/bash

gsed -e 's/\xC3\x84/\xC4/g' $1 |\
  gsed -e 's/\xC3\x85/\xC5/g' |\
  gsed -e 's/\xC3\x96/\xD6/g' |\
  gsed -e 's/\xC3\xA4/\xE4/g' |\
  gsed -e 's/\xC3\xA5/\xE5/g' |\
  gsed -e 's/\xC3\xB6/\xF6/g' |\
  gsed -e 's/\xC3\xA9/\xE9/g' |\
  gsed -e 's/\xC3\x89/\xC9/g' > $1.res
mv $1.res $1
