#!/usr/bin/python


from matplotlib.pyplot import figure, show

from numpy import arange, sin, pi

import struct
import os
import time
import sys
import numpy as np 

if (len(sys.argv) != 2):
	print 'usage == python 1)filein name  '
	exit(0)

file_name = sys.argv[1]
fmt = "32h"

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
	'''
		while(True):
				c=f.read(2)
				d=struct.unpack(fmt1,c)
				if d[0]==0x1a41:
						print ' Got the header '
						break
		'''
	fig = figure()
	fig.canvas.set_window_title('plot at ' + timestamp)


	ax1 = fig.add_subplot(221)
	ax2 = fig.add_subplot(222)
	ax3 = fig.add_subplot(223)
	ax4 = fig.add_subplot(224)


	title = 'Actuator-1'
	ax1.set_title(str(title))
	ax1.set_xlabel(' samples count  1sec=400 samples')
	ax1.set_ylabel('  In degrees')
	# ax1.set_facecolor("#000000")


	
	# title='Plot --'+timestamp
	title = 'Actuator-2'
	ax2.set_title(str(title))
	ax2.set_xlabel(' samples count  1sec=400 samples')
	ax2.set_ylabel('  In degrees')
	# ax2.set_facecolor("#000000")

	title = 'Actuator-3'
	ax3.set_title(str(title))
	ax3.set_xlabel(' samples count  1sec=400 samples')
	ax3.set_ylabel(' In degrees')
	# ax3.set_facecolor("#000000")

	title = 'Actuator-4'
	ax4.set_title(str(title))
	ax4.set_xlabel(' samples count  1sec=400 samples')
	ax4.set_ylabel(' In degrees')
	# ax4.set_facecolor("#000000")


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
	abs_scale_factor = 22.755  # 0.043945312  #360/8192

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
			#data = struct.unpack(timetag_fmt + header_fmt, bytes[0:timetag_size + header_size])
			data1 = struct.unpack('16h', bytes[timetag_size + header_size + 8: timetag_size + header_size + 8 + 32])
			data2 = struct.unpack('>4h', bytes[timetag_size + header_size + 8 + 32: timetag_size + header_size + 8 + 32 + 8])
			#print data1, data2

			t.append(y)
			
			
                        #Absolute Feedback
			li[0][0].append(float(data1[12]) / abs_scale_factor)
			li[0][1].append(float(data1[13]) / abs_scale_factor)
			li[0][2].append(float(data1[14]) / abs_scale_factor)
			li[0][3].append(float(data1[15]) / abs_scale_factor)
			
			
                        #Incremental Position                                                                                                                                                    
			li[1][0].append(float(data1[4]) / scale_factor)
			li[1][1].append(float(data1[5]) / scale_factor)
			li[1][2].append(float(data1[6]) / scale_factor)
			li[1][3].append(float(data1[7]) / scale_factor)
			
                        #current
			li[3][0].append(float(data1[8]) / current_scale_factor)
			li[3][1].append(float(data1[9]) / current_scale_factor)
			li[3][2].append(float(data1[10]) / current_scale_factor)
			li[3][3].append(float(data1[11]) / current_scale_factor)


                        #OBC Command
			li[2][0].append(float(data1[0]) / scale_factor)
			li[2][1].append(float(data1[1]) / scale_factor)
			li[2][2].append(float(data1[2]) / scale_factor)
			li[2][3].append(float(data1[3]) / scale_factor)

			outfile.write(','.join(str(float(i) / scale_factor) for i in data2))
			outfile.write('\n')
			
			outfile1.write(','.join(str(float(i)) for i in data2))
			outfile1.write('\n')

			

		else:
			print ' Ended'
			outfile1.close()
			outfile.close()
			break

	ax1.grid(True)
	#current
	ax1.plot(t, li[3][0], color='y')
	# command
	ax1.plot(t, li[0][0], '-', color='b')
	# incremnetal Command
	ax1.plot(t, li[1][0], '-', color='g')
	# obc_demand
	ax1.plot(t, li[2][0], color='r')



	ax2.grid(True)
	# current
	ax2.plot(t, li[3][1], color='y')
	# command
	ax2.plot(t, li[0][1], '-', color='b')
	# incremnetal Command
	ax2.plot(t, li[1][1], '-', color='g')
	# obc_demand
	ax2.plot(t, li[2][1], color='r')

	ax3.grid(True)
	# current
	ax3.plot(t, li[3][2], color='y')
	# command
	ax3.plot(t, li[0][2], '-', color='b')
	# incremnetal Command
	ax3.plot(t, li[1][2], '-', color='g')
	# obc_demand
	ax3.plot(t, li[2][2], color='r')

	ax4.grid(True)
	#current
	ax4.plot(t, li[3][3], color='y')
	# command
	ax4.plot(t, li[0][3], '-', color='b')
	# incremnetal Command
	ax4.plot(t, li[1][3], '-', color='g')
	# obc_demand
	ax4.plot(t, li[2][3], color='r')

	tt1 = np.array(li[3][0])	
	print 'max, ', max(li[3][0]) , '   ', min(li[3][0], key=abs), 'mean', np.mean(np.mean(tt1 ** 2)), 'rms value', np.sqrt(np.mean(tt1 ** 2))

	tt1 = np.array(li[3][1])	
	print 'max, ', max(li[3][1]) , '   ', min(li[3][1], key=abs), 'mean', np.mean(np.mean(tt1 ** 2)), 'rms value', np.sqrt(np.mean(tt1 ** 2))
	
	tt1 = np.array(li[3][2])	
	print 'max, ', max(li[3][2]) , '   ', min(li[3][2], key=abs), 'mean', np.mean(np.mean(tt1 ** 2)), 'rms value', np.sqrt(np.mean(tt1 ** 2))

	tt1 = np.array(li[3][3])	
	print 'max, ', max(li[3][3]) , '   ', min(li[3][3], key=abs), 'mean', np.mean(np.mean(tt1 ** 2)), 'rms value', np.sqrt(np.mean(tt1 ** 2))

	show()

except struct.error as st:
	print 'NO Data  ', st
