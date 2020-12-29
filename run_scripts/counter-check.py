#!/usr/bin/python


import matplotlib.pyplot as plt
from numpy import arange, sin, pi
import struct
import os
import time
import sys


if (len(sys.argv) != 3):
    print 'usage == python 1)CCU file name 2) OBC Filename'
    sys.exit(0)


ccufile = sys.argv[1]
obcfile = sys.argv[2]

if os.path.islink(obcfile):
    real_path = os.readlink(obcfile)
    time1 = os.path.getctime(real_path)
else:
    time1 = os.path.getctime(obcfile)

timestamp = time.strftime('%d-%m-%Y %H:%M:%S', time.localtime(time1))
f = open(obcfile, "rb")

ccu_packetsize = 46
obc_packetsize = 74

try:
    fig, ax1 = plt.subplots()
    fig.canvas.set_window_title('plot at ' + timestamp)

    #legend = ax1.legend(loc='upper center', shadow=True, fontsize='x-large')
    #legend.get_frame().set_facecolor('C0')
    ax1.grid(True)

    ccufp = open(ccufile, "rb")
    obcfp = open(obcfile, "rb")

    li = [[], []]
    ti = [[], [], [], []]

    # linecount = os.stat(ccufile).st_size / ccu_packetsize
    # print 'ccu count', linecount
    # for y in range(linecount):
    #     bytes = ccufp.read(ccu_packetsize)
    #     if bytes:
    #         data = struct.unpack("=I2BHHI4BH22B2BH", bytes)
    #         li[1].append(data[10])
    #         li[0].append(y)

    linecount = os.stat(obcfile).st_size / obc_packetsize
    print 'obc lcount--', linecount

    tap = ta = 0
    tbp = tb = 0
    act_count = 0

    tempfp = open('/tmp/temp.bin', "wb+")

    for y in range(linecount):
        bytes = obcfp.read(obc_packetsize)
        if bytes:
            data = struct.unpack("=HHI4BH22B2BH12H", bytes[10:74])
            if data[0] != 0:
                tempfp.write(bytes)
                act_count += 1
    tempfp.close()
    obcfp.close()

    print 'sample count ', act_count
    print 'enter the start and end index of lines'
    start = int(raw_input())
    end = int(raw_input())
    print start, end

    tempfp = open("/tmp/temp.bin", "rb")

    print 'tell---', tempfp.tell()
    tempfp.seek(start * obc_packetsize, os.SEEK_SET)
    print 'tell---', tempfp.tell()

    # for y in range(0, start, 1):
    #     obcfp.read(obc_packetsize)

    pkta = pktb = 0
    for y in range(start, end):
        bytes = tempfp.read(obc_packetsize)
        if bytes:
            data = struct.unpack("=HHI4BH22B2BH12H", bytes[10:74])
            if data[4] == 1:
                ta = data[7]
                if tap == ta:
                    continue
                ti[1].append(ta - tap)
                pkta += 1
                ti[0].append(pkta)
                tap = ta

            elif data[4] == 2:
                tb = data[7]
                if tbp == tb:
                    continue
                pktb += 1
                ti[3].append(tb - tbp)
                ti[2].append(pktb)
                tbp = tb

            #print data
        else:
            continue

    tempfp.close()
    ccufp.close()


    #ax1.plot(li[1])
    ax1.plot(ti[0], ti[1])
    ax1.plot(ti[2], ti[3])

    plt.show()

except struct.error as st:
    print 'NO Data  ', st
