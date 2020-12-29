#!/usr/bin/python


#from matplotlib.pyplot import figure, show

from numpy import arange, sin, pi


import openpyxl
from openpyxl import load_workbook
from openpyxl import Workbook
from openpyxl.chart import LineChart,Reference, Series
from openpyxl.chart.shapes import GraphicalProperties

import struct
import os
import time
import sys

from ReportVarList import *

timetag_fmt = 'L'
header_fmt = 'H'


if(len(sys.argv) != 2):
    print 'usage == python 1)Command Name '
    exit(0)

cmd_name = sys.argv[1]
try:
        input_file_list=logfile_map[cmd_name]
        for i in range(len(input_file_list)):
                real_path = os.readlink(input_file_list[i])
               
                time1 = os.path.getctime(real_path)
                timestamp = time.strftime('%d-%m-%Y-%H-%M-%S', time.localtime(time1))
               
                index = index_map[cmd_name]
           

                fd_out = open(sheet_name_map[index[i]]+str('.csv'), "w")


                timetag_size = struct.calcsize(timetag_fmt)
                header_size = struct.calcsize(header_fmt)
                packet_size = timetag_size + header_size + 64
                    
                print 'timetag_size, ', timetag_size, 'header_size ', header_size
      
                line_count = os.stat(input_file_list[i]).st_size / packet_size
                print 'line count ', line_count , 'input_file_list[i]' , input_file_list[i]
                
                f = open(input_file_list[i], "rb")
               
                for y in range(line_count):
                        bytes = f.read(packet_size)
                        if bytes:
                               
                                data = struct.unpack(fmt_list[index[i]], bytes[timetag_size + header_size:  74])
                                #print len(data)
                                ti = []

                                try:
                                        for k in range(len(index_list[index[i]])):
                                                val = ""
                                                s1 = ""
                                                
                                                if(scale_list[index[i]][k] == -1):
                                                        val = data[index_list[index[i]][k]]
                                                elif(scale_list[index[i]][k] == -2):
                                                        val = (float(data[index_list[index[i]][k]] - 32768) * 20 ) / 65536
                                                elif(scale_list[index[i]][k] == -3):
                                                        val = float(int(data[index_list[index[i]][k]]) << 16 | int(data[index_list[index[i]][k+1]])) * 0.0025
                                                elif(scale_list[index[i]][k] == -4):
                                                        continue; 
                                                else:
                                                        val = data[index_list[index[i]][k]] * scale_list[index[i]][k]
                                                
                                                if (display_list[index[i]][k] == -1):
                                                    s1 = val
                                                
                                                else:
                                                    s1 = display_list[index[i]][k] % (val)
                                                    
                                                ti.append(s1)
                                                ti.append('\t')

                                        ti.append('\n')
                                        for jj in ti:
                                         fd_out.write(str(jj))                                         
                                                
                                except IndexError as I:
                                        print "Error  ",I
					break;

                        else:
                            print ' Ended'
                            break
                            
                print "Out"
                fd_out.close()

                print "End"


except KeyError as K:
        print "KeyError:",K
        
except OSError as E:
        print "OSError: ",E


