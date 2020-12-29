#!/bin/sh

gnome-terminal -e 'sudo ./mtirq' &
gnome-terminal -e 'tail -f /tmp/mtstack.txt' &
