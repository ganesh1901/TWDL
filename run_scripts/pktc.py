
from matplotlib.pyplot import figure, show

from numpy import arange, sin, pi

import struct
import os
import time
import sys
import binascii

if (len(sys.argv) != 2):
    print 'usage == python 1)filein name '
    exit(0)

file_name = sys.argv[1]

timetag_fmt = 'L'
header_fmt = 'H'

if os.path.islink(file_name):
    real_path = os.readlink(file_name)
    time1 = os.path.getctime(real_path)
else:
    time1 = os.path.getctime(file_name)


timestamp = time.strftime('%d-%m-%Y %H:%M:%S', time.localtime(time1))

timetag_size = struct.calcsize(timetag_fmt)
header_size = struct.calcsize(header_fmt)
packet_size = timetag_size + header_size + 64

print 'packet_size', packet_size

line_count = os.stat(file_name).st_size / packet_size
print 'line count ', line_count

f = open(file_name, "rb")

pktAmiss = 0
pktAtotal = 0
pktAPreSeqNum = 0
pcount=0
pseq=0
for y in range(line_count):
	if y > 10:
		bytes = f.read(packet_size)
		if bytes:
			a = struct.unpack("=HHHHBB18H", bytes[timetag_size + header_size:  timetag_size + header_size + 46])

			msgId = a[5]
			currSeqNumber = a[4] #>> 8 | ((a[5] & 0xff) << 8)

			

			if currSeqNumber!=0 and pseq!=currSeqNumber:
				pcount+=1
				pseq=currSeqNumber
				print "Seq=", currSeqNumber
				

			if (msgId == 0x03):  # Packet A
				if (currSeqNumber < pktAPreSeqNum):
					pktAPreSeqNum = currSeqNumber

				if (pktAtotal == 0):
					pktAPreSeqNum = currSeqNumber
				else:
					if (pktAPreSeqNum < currSeqNumber) and pktAPreSeqNum + 1 != currSeqNumber:

						#print 'pre=   ', pktAPreSeqNum, '      current=', currSeqNumber
						pktAmiss += currSeqNumber - pktAPreSeqNum - 1
						pktAPreSeqNum = currSeqNumber
					else:
						if (pktAPreSeqNum != currSeqNumber):
							pktAPreSeqNum += 1
				pktAtotal += 1


print "Total Packet Miss===",pktAmiss
