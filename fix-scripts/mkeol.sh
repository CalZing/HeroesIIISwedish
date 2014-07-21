#!/bin/bash

gsed -z -e 's/[^\x0D\x0A]//g' en/$1 > $1.eol
