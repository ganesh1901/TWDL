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

abs_path = os.getenv("PWD")
print abs_path

# abs_path = "/home/testbench/Controller/run_scripts/"

today = time.localtime()
dirname = "%03d-%02d%02d%02d" % (today.tm_yday, today.tm_hour, today.tm_min, today.tm_sec)

print dirname

if os.path.isdir(abs_path + '/../out/' + dirname) == False:
    os.mkdir(abs_path + '/../out/' + dirname)

output_filename = ['Navdata-17.csv', 'Navdata-18.csv', 'Navdata-19.csv', 'Navdata-20.csv', 'Navdata-21.csv',
                   'Navdata-22.csv']

fmt_list = ["15f2H", "13f2H2f", "16f", "12f3I2H", "f32BH4BHI8h", "13fiI2H", "13fh5H", "11f2h2H3f", "5H12h12Hf",
            '2HI20h6H2H']
timetag_fmt = 'L'
header_fmt = 'H'

nav_cmd_list = [0x0e20, 0x0e40, 0x0e60, 0x0e80, 0x0ea0, 0x0ec0]

# fd list to save files
outfile_fd = []

for i in output_filename:
    gd = open(abs_path + '/../out/' + dirname + '/' + i, 'w')
    outfile_fd.append(gd)


def Process_File(input_filename, fmt_index, outfd_index):
    fdin = open(abs_path + '/' + input_filename, 'rb')

    print 'fmt index ', fmt_index, fmt_list[fmt_index]

    timetag_size = struct.calcsize(timetag_fmt)
    header_size = struct.calcsize(header_fmt)
    packet_size = timetag_size + header_size + 64

    print 'packet_size', packet_size

    line_count = os.stat(abs_path + '/' + input_filename).st_size / packet_size
    print 'line count ', line_count

    fmt_specifier = ''
    fdout = ''

    for y in range(line_count):

        bytes = fdin.read(packet_size)
        li = []
        data1 = struct.unpack(timetag_fmt + header_fmt, bytes[0:timetag_size + header_size])
        if data1:
            if data1[1] in nav_cmd_list:
                fmt_specifier = fmt_list[nav_cmd_list.index(data1[1])]
                fdout = outfile_fd[nav_cmd_list.index(data1[1])]
            else:
                fmt_specifier = fmt_list[fmt_index]
                fdout = outfile_fd[outfd_index]


            if fmt_specifier != '':
                fmt_specifier = fmt1
            if fdout != '':
                fdout = open(abs_path + '/../out/' + dirname + '/' + outfile, 'w')


        else:
            print '**************** file reached out **************'
            sys.exit()

        timestamp = datetime.fromtimestamp(data1[0] / 1e6).strftime('%d-%m-%Y %H:%M:%S %f')
        li.append(timestamp)
        li.append(data1[1])

        data = struct.unpack(fmt_specifier, bytes[timetag_size + header_size: packet_size])
        # print 'length of tuple', len(data)

        if bytes:
            fdout.write(',')
            fdout.write(str(timestamp))
            fdout.write(',')
            fdout.write(str(data1[1]))
            fdout.write(',')
            fdout.write(','.join(str(i) for i in data))
            fdout.write('\n')

    fdin.close()
    # os.unlink(abs_path + '/' + input_filename)


if __name__ == '__main__':

    count = 0

    for i in input_name:
        Process_File(i, 5 + count, 5 + count)
        count += 1

    for i in outfile_fd:
        i.close()



