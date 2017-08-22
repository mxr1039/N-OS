#!/bin/bash

#
# boom.sh
# a bash implementation of the boom program
# author: bjr
# date: 14 dec 2016
#

argc=$#
if  [[ "$argc" -ne 1 ]] ; then
	echo "usage: boom _number_"
	exit 1
fi
cnt=$1
if [[ "$cnt" -le 0 ]] ; then
	cnt=0
fi
while true; do
	if [[  "$cnt" -eq 0 ]] ; then
		echo "Boom!"
		exit 1 
	else
		echo "$cnt"
		cnt=$((cnt-1))
		sleep 1
	fi
done

