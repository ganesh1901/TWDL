import os
import sys

if len(sys.argv) != 2:
    print 'Usage is file name '
    sys.exit()

filename = sys.argv[1]
# !/usr/bin/python

import struct
import os
import time
import sys
from datetime import datetime

timetag_fmt = 'L'
header_fmt = 'H'

timetag_size = struct.calcsize(timetag_fmt)
header_size = struct.calcsize(header_fmt)
packet_size = timetag_size + header_size + 64


command_list = [0x2107, 0x0406]

file_name_list = ["ins-data-load.txt", "ins-data-dump.txt"]
fmt_list = ["=HH2HddffffffHH6H", "=HH2HddffffffHH6H"]


f1 = open(filename)

line_count = os.stat(filename).st_size / packet_size
print 'line count ', line_count


for y in range(line_count):
    try:
        buff = f1.read(packet_size)
        if buff:
            data = struct.unpack('2H', buff[10:14])
            print hex(data[0]), '  ', hex(data[0])

            for i in range(len(command_list)):

                if data[0] == command_list[i]:

                    fd_in = open(file_name_list[i], 'a+')
                    data1 = struct.unpack(fmt_list[i], buff[10:74])
                    print data1
                    for jj in data1:
                        fd_in.write(str(jj))
                        fd_in.write('\t')
                    fd_in.write('\n')
                    fd_in.close()


    except struct.error:
        pass









