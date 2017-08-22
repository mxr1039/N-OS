#!/bin/bash

#
# remove-files.sh
# 
# author: bjr
# date: 14 dec 2016
#

argc=$#
if  [[ "$argc" -ne 2 ]] ; then
	echo "usage: touch-files _name_ _number_"
	exit 0
fi
name=$1
cnt=$2
if [[ "$cnt" -le 0 ]] ; then
	cnt=0
fi

while true; do
	if [[  "$cnt" -eq 0 ]] ; then
		echo "done"
		exit 0
	else
		rm "${name}_$cnt.f"
		cnt=$((cnt-1))
		#sleep 1
	fi
done

