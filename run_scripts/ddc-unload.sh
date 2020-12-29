#!/bin/sh 


sudo rmmod acexusb
if [ $? -eq 0 ]
then 
	echo 'ddc unload successfully'
else
	echo ' unload failure '
fi
