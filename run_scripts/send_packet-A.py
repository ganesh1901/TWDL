#!/usr/bin/python

import os
import sys
import struct
import time

import flatbuffers
import ViewController.Interface_Type
import ViewController.IdentifyHardwareInterface
import ViewController.Mil_1553
import ViewController.IP_RRU
import ViewController.USB_RRU
import ViewController.Transaction
global rsp_fd


def PrepareObject(interface_type, payload):
    #print 'faltc ', interface_type, '     ', payload
    try:
        builder = flatbuffers.Builder(512)
        if interface_type == 'IPCARD':
            ViewController.IP_RRU.IP_RRUStart(builder)
            if payload[0] == 1:
                ViewController.IP_RRU.IP_RRUAddIType(builder, ViewController.Interface_Type.Interface_Type().DOP)
            elif payload[0] == 0:
                ViewController.IP_RRU.IP_RRUAddIType(builder, ViewController.Interface_Type.Interface_Type().DIP)
            else:
                ViewController.IP_RRU.IP_RRUAddIType(builder, ViewController.Interface_Type.Interface_Type().ADC)

            ViewController.IP_RRU.IP_RRUAddSlot(builder, payload[1])
            ViewController.IP_RRU.IP_RRUAddPort(builder, payload[2])
            ViewController.IP_RRU.IP_RRUAddChannel(builder, payload[3])
            ViewController.IP_RRU.IP_RRUAddValue(builder, payload[4])
            ors = ViewController.IP_RRU.IP_RRUEnd(builder)

            ViewController.Transaction.TransactionStart(builder)
            ViewController.Transaction.TransactionAddQueryOnInterfaceType(builder,
                                                                          ViewController.IdentifyHardwareInterface.IdentifyHardwareInterface().IP_RRU)
            ViewController.Transaction.TransactionAddQueryOnInterface(builder, ors)
            orc = ViewController.Transaction.TransactionEnd(builder)

        elif interface_type == 'MIL1553':

            ViewController.Mil_1553.Mil_1553StartPayloadVector(builder, len(payload[4]))
            for i in range(len(payload[4])):
                builder.PrependUint8(payload[4][len(payload[4]) - i - 1])
            cmd_payload = builder.EndVector(len(payload[4]))

            ViewController.Mil_1553.Mil_1553Start(builder)
            ViewController.Mil_1553.Mil_1553AddRt(builder, payload[0])
            ViewController.Mil_1553.Mil_1553AddSubaddress(builder, payload[1])
            ViewController.Mil_1553.Mil_1553AddDirection(builder, payload[2])
            ViewController.Mil_1553.Mil_1553AddWordcount(builder, payload[3])

            ViewController.Mil_1553.Mil_1553AddPayload(builder, cmd_payload)
            ViewController.Mil_1553.Mil_1553AddFrequency(builder, payload[5])
            ors = ViewController.Mil_1553.Mil_1553End(builder)

            ViewController.Transaction.TransactionStart(builder)
            ViewController.Transaction.TransactionAddQueryOnInterfaceType(builder,
                                                                          ViewController.IdentifyHardwareInterface.IdentifyHardwareInterface().Mil_1553)
            ViewController.Transaction.TransactionAddQueryOnInterface(builder, ors)
            orc = ViewController.Transaction.TransactionEnd(builder)


        elif interface_type == 'Ethernet':
            print 'In etherenet '
            #print payload[1], 'size', payload[1]

        builder.Finish(orc)
        buf = builder.Output()

        #print '^^^^^^^^^^^^^^^^^^^^size is', len(buf)
        os.write(rsp_fd, buf)


    except TypeError as type_err:
        print  '*****', type_err



if(len(sys.argv) != 2):
    print 'usaage 1)python 2) i\p file'
    sys.exit(0)

filename = sys.argv[1]



timetag_fmt = 'L'
header_fmt = 'H'
timetag_size = struct.calcsize(timetag_fmt)
header_size = struct.calcsize(header_fmt)
packet_size = timetag_size + header_size + 64
f = open(filename, "rb")

line_count = os.stat(filename).st_size / packet_size
#print 'line count ', line_count


rsp_fd = os.open("/tmp/rsp_fifo", os.O_RDWR | os.O_NONBLOCK)
#print 'rsp fifo ', rsp_fd

for y in range(line_count):
    bytes = f.read(packet_size)
    if bytes:
        li = []
        li.append(1)
        li.append(4)
        li.append(0)
        li.append(32)
        li.append(bytearray(bytes[10:74]))
        li.append(1)
        PrepareObject("MIL1553", li)
        time.sleep(.1)



