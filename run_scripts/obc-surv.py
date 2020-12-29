#!/usr/bin/python


from matplotlib.pyplot import figure, show

from numpy import arange, sin, pi

import struct
import os
import time
import sys
import binascii

if (len(sys.argv) != 3):
	print 'usage == python 1)filein name  2) time interval '
	exit(0)

file_name = sys.argv[1]
fmt = "32h"

interval = float(sys.argv[2])

packet_size = struct.calcsize(fmt)
print 'calcsize', packet_size
timetag_fmt = 'L'
header_fmt = 'H'

if os.path.islink(file_name):
	real_path = os.readlink(file_name)
	time1 = os.path.getctime(real_path)
else:
	time1 = os.path.getctime(file_name)

timestamp = time.strftime('%d-%m-%Y %H:%M:%S', time.localtime(time1))

f = open(file_name, "rb")

fmt1 = "=H"
try:

	fig = figure(1)
	fig.canvas.set_window_title('plot at ' + timestamp)
	ax1 = fig.add_subplot(111)

	timetag_size = struct.calcsize(timetag_fmt)
	header_size = struct.calcsize(header_fmt)
	packet_size = timetag_size + header_size + 64

	print 'packet_size', packet_size

	line_count = os.stat(file_name).st_size / packet_size
	print 'line count ', line_count

	t = [[], []]

	outfile = open('/tmp/test.csv', 'w+')
	outfile1 = open('/tmp/test_raw.csv', 'w+')

	xx = xx1 = 0
	for y in range(line_count):

		bytes = f.read(packet_size)

		if bytes:
			dd = struct.unpack("=5H12hHI2H2B7H2H", bytes[10:74])
			xx = round(float(int(dd[len(dd) - 2]) << 16 | int(dd[len(dd) - 1])) * 0.0025, 4)
			t[1].append(xx - xx1)
			xx1 = xx
			# for ii in range(len(dd) - 2):
			# 	outfile.write("%4.4f" % (dd[ii]))
			# 	outfile.write('\t')
			outfile.write("%4.4f" % xx1)
			outfile.write('\t')

			t[0].append(y * interval)
			outfile.write("%4.4f" % float(y+1* interval))
			outfile.write('\t')
			outfile.write('\n')

		else:
			print ' Ended'
			break

			outfile.close()
			outfile1.close()

	ax1.grid(True)
	ax1.plot(t[0], t[1])
	show()

except struct.error as st:
	print 'NO Data  ', st
