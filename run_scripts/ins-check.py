#!/usr/bin/python
from matplotlib.axes import Axes
from matplotlib.pyplot import figure, show

from numpy import arange, sin, pi

import struct
import os
import time
import sys
import binascii

if (len(sys.argv) != 4):
	print 'usage == python 1)filein name  2) interval time 3) BUS-Monitor(0)/Program(1)'
	exit(0)

file_name = sys.argv[1]
time_interval = float(sys.argv[2])
control_flag = int(sys.argv[3])
fmt = "13fh5H"

timetag_fmt = 'L'
header_fmt = 'H'

timetag_size = struct.calcsize(timetag_fmt)
header_size = struct.calcsize(header_fmt)

if control_flag == 0:
	packet_size = 64
else:
	packet_size = timetag_size + header_size + 64

print 'calcsize', packet_size

if os.path.islink(file_name):
	real_path = os.readlink(file_name)
	time1 = os.path.getctime(real_path)
else:
	time1 = os.path.getctime(file_name)

timestamp = time.strftime('%d-%m-%Y %H:%M:%S', time.localtime(time1))

f = open(file_name, "rb")
line_count = os.stat(file_name).st_size / packet_size
print 'line count ', line_count

fig = figure(1)

y = []
x = []
ax1 = fig.add_subplot(111)
ax1.grid(True)
try:
	bytes = f.read(packet_size)
	if control_flag:
		data1 = struct.unpack(fmt, bytes[timetag_size + header_size: timetag_size + header_size + 64])
	else:
		data1 = struct.unpack(fmt, bytes[0: 64])


	for t in range(line_count-1):

		bytes = f.read(packet_size)
		if bytes:
			if control_flag:
				data = struct.unpack(fmt, bytes[timetag_size + header_size: timetag_size + header_size + 64])
			else:
				data = struct.unpack(fmt, bytes[0:64])

			diff = round(data[0], 4) - round(data1[0], 4)
			print round(data[0], 4), diff
			# if(diff > time_interval):
			# 	print "line number %d -- value %f diff== %f" % (t, round(data[0], 4), diff)
			x.append(diff)
			y.append(t * time_interval)
			#print x[t], y[t]
			data1 = data

	#print x

	ax1.plot(y, x)
	show()


except struct.error as st:
	print 'NO Data  ', st
