#!/bin/sh

if [ $# -ne 1 ]
then
	echo 'usage 1)./sh  2)executable name '
else
	ps H -C $1 -o 'pid tid cmd comm'
fi
