#!/usr/bin/python


from matplotlib.pyplot import figure, show

from numpy import arange, sin, pi

import struct
import os
import time
import sys
import binascii

if (len(sys.argv) != 3):
	print 'usage == python 1)filein name  2) Act number '
	exit(0)

file_name = sys.argv[1]
fmt = "32h"

act_no = int(sys.argv[2])

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
	title = 'Actuator ---'+str(act_no)
	ax1.set_title(title)
	ax1.set_xlabel(' samples count  1sec=400 samples')
	ax1.set_ylabel('  In degrees')
	# ax1.set_facecolor("#000000")



	fig.text(0.9, 0.99, '---Abs Fb', fontsize=14, color='b', horizontalalignment='right', verticalalignment='top')
	fig.text(0.9, 0.97, '---Inc Fb', fontsize=14, color='g', horizontalalignment='right', verticalalignment='top')
	fig.text(0.9, 0.95, '---obc-demand', fontsize=14, color='r', horizontalalignment='right', verticalalignment='top')
	fig.text(0.9, 0.93, '---current', fontsize=14, color='y', horizontalalignment='right', verticalalignment='top')

	t = []
	loopback = []
	li = [[[], [], [], []], [[], [], [], []], [[], [], [], []], [[], [], [], []]]
	x = 100
	y = 0

	scale_factor = 182.044 #0.00549 #1/1092
	current_scale_factor = 525  # 0.001904762  #1/525
	abs_scale_factor = 22.75555 # 0.043945312  #360/8192

	timetag_size = struct.calcsize(timetag_fmt)
	header_size = struct.calcsize(header_fmt)
	packet_size = timetag_size + header_size + 64

	print 'packet_size', packet_size

	line_count = os.stat(file_name).st_size / packet_size
	print 'line count ', line_count

	fmt_specifier = ''
	fdout = ''
	t = []

	outfile = open('/tmp/test.csv', 'w+')
	outfile1 = open('/tmp/test_raw.csv', 'w+')

	for y in range(line_count):

		bytes = f.read(packet_size)


		if bytes:
                        data = struct.unpack('2H', bytes[timetag_size + header_size : timetag_size + header_size + 4])
                        if data[1] == 0x4543:
			        data1 = struct.unpack('16h', bytes[timetag_size + header_size + 8: timetag_size + header_size + 8 + 32])
			        data2 = struct.unpack('>4h', bytes[timetag_size + header_size + 8 + 32: timetag_size + header_size + 8 + 32 + 8])
			        data3 = struct.unpack('=2H', bytes[70:74])
			        obc_time = float(int(data3[0]) << 16 | int(data3[1])) * 0.0025

                                data4 = struct.unpack('2HI16h', bytes[timetag_size + header_size : timetag_size + header_size + 40])
                                data5 = struct.unpack('2H', bytes[timetag_size + header_size + 54 : timetag_size + header_size + 58])
					
                                
                                temp = []
			        for i in data4:
			                temp.append(str(hex(i)))
				for i in data5:
			                temp.append(str(hex(i)))
				
			        print temp

			        t.append(y * 0.0025)

			        li[0][0].append(float(data1[act_no+11]) / abs_scale_factor)

			        li[1][0].append(obc_time)
			        #li[1][0].append(float(data1[act_no+3]) / scale_factor)

			        #li[3][0].append(float(-data1[act_no+7]) / current_scale_factor)
			        li[3][0].append(data5[0] & 0x000f)

			        li[2][0].append(float(data1[act_no-1]) / scale_factor)

			        outfile.write(','.join(str(float(i) / scale_factor) for i in data1))
			        outfile.write(',')
			        outfile.write(','.join(str(float(i) / scale_factor) for i in data2))
			        outfile.write('\n')

			        outfile1.write(','.join(str(i) for i in data1))
			        outfile1.write(',')
			        outfile1.write(','.join(str(i) for i in data2))
			        outfile1.write(',')
			        outfile1.write(str(obc_time))
			        outfile1.write('\n')


		else:
			print ' Ended'
			break

			outfile.close()
			outfile1.close()



	ax1.grid(True)
	# command
	#ax1.plot(t, li[0][0], '-', color='b')

	# incremnetal Command
	ax1.plot(t, li[1][0], '-', color='g')

	# obc_demand
	#ax1.plot(t, li[2][0], color='r')

	# current
	ax1.plot(t, li[3][0], color='y')

	show()

except struct.error as st:
	print 'NO Data  ', st