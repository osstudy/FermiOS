#!/bin/sh

find {src,include,Makefile} -not -type d | xargs -i wc -l '{}' | awk '{sum+=$1} END {print "TOTAL LINES:"; print sum}'
cloc {src,include,Makefile}
