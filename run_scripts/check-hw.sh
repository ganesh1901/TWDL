#!/bin/sh

status=`lsmod | grep acex`
echo ' DDC load status ', ${status} 


if [ ${status} -ne ' ' ]
then
	echo 'DDC MODULE LOADED' 
fi


