#!/bin/bash

find {src,include,Makefile} -type f \
	| xargs -i grep -H -T -n -E --color=always "TODO|FIXME" '{}' \
	| awk '{ printf $1 $2; $1=$2=""; print $0}'
