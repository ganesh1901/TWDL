#!/bin/sh

tar cf ../APP.tar main.py *.pyc ViewController/*.pyc  ../bin/simulator ../conf/devconf.cfg ../inp/sample.xlsx ;
md5sum ../APP.tar  >/tmp/cfg 
