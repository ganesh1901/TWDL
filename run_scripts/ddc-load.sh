#!/bin/sh 
cd $HOME/PMCS/run_scripts
sudo insmod acexusb.ko
if [ $? -ne 0 ]
then 
	echo 'load problem '
else 
	echo ' loading successfully '
fi

sudo ./ddccm < ddc-input
