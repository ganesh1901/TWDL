
from matplotlib.pyplot import figure, show

from numpy import arange, sin, pi

import struct
import os
import time
import sys

import flatbuffers
import ViewController.Interface_Type
import ViewController.IdentifyHardwareInterface
import ViewController.Mil_1553
import ViewController.IP_RRU
import ViewController.USB_RRU
import ViewController.Transaction


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


tx_fifo = "/tmp/rsp_fifo"
if not os.access(tx_fifo, 6):
    os.mkfifo(tx_fifo, 0600)
rsp_fd = os.open(tx_fifo, os.O_RDWR | os.O_NONBLOCK)
print 'rsp fifo ', rsp_fd


sur_fifo = "/tmp/sur_fifo"
if not os.access(sur_fifo, 6):
    os.mkfifo(sur_fifo, 0600)
sur_fd = os.open(sur_fifo, os.O_RDWR | os.O_NONBLOCK)
print 'rsp fifo ', sur_fd


for y in range(line_count):
    data = f.read(packet_size)

    cmd = struct.unpack('=H', data[8:10])

    rt = (cmd[0]
    print rt




