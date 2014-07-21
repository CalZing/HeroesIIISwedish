#!/bin/bash

gsed -e 's/\xC5/\xC3\x84/g' $1 |\
  gsed -e 's/\xC4/\xC3\x85/g' |\
  gsed -e 's/\xD6/\xC3\x96/g' |\
  gsed -e 's/\xE5/\xC3\xA4/g' |\
  gsed -e 's/\xE4/\xC3\xA5/g' |\
  gsed -e 's/\xE9/\xC3\xA9/g' |\
  gsed -e 's/\xC9/\xC3\x89/g' |\
  gsed -e 's/\xF6/\xC3\xB6/g' > $1.uni
mv $1.uni $1
