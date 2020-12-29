#!/usr/bin/python


from matplotlib.pyplot import figure, show

#from numpy import arange, sin, pi


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


if(len(sys.argv) != 4):
    print 'usage == python 1)Command Name 2)PMCS/demo 3) timediff'
    exit(0)

cmd_name = sys.argv[1]
try:
	fig = figure(1)
	ax1 = fig.add_subplot(111)
        input_file_list=logfile_map[cmd_name]
        for i in range(len(input_file_list)):
                real_path = os.readlink(input_file_list[i])
               
                time1 = os.path.getctime(real_path)
                timestamp = time.strftime('%d-%m-%Y-%H-%M-%S', time.localtime(time1))
               
                index = index_map[cmd_name]
           

                fd_out = open(sheet_name_map[index[i]]+str('.csv'), "w")
		
		if int(sys.argv[2]) == 0:
	
        	        timetag_size = 0#struct.calcsize(timetag_fmt)
                	header_size = 0#struct.calcsize(header_fmt)
		else:
        	        timetag_size = struct.calcsize(timetag_fmt)
                	header_size = struct.calcsize(header_fmt)

                packet_size = timetag_size + header_size + 64
                    
                print 'timetag_size, ', timetag_size, 'header_size ', header_size
      
                line_count = os.stat(input_file_list[i]).st_size / packet_size
                print 'line count ', line_count , 'input_file_list[i]' , input_file_list[i]
                
                f = open(input_file_list[i], "rb")
              	miss_count = 0
		dup_count = 0
		li1 = []
                for y in range(line_count):
                        bytes = f.read(packet_size)
                        if bytes:
                                if int(sys.argv[2]) == 0:
		                        data = struct.unpack(fmt_list[index[i]], bytes[0: 64])
				else:
                                	data = struct.unpack(fmt_list[index[i]], bytes[timetag_size + header_size:  74])                                
                                #print len(data)
                               	t1 = data[16]
				li1.append(data[16])
                                if y != 0:
					diff = t1-last_value
					if(diff > 1):
						miss_count += 1
						print(" miss line %d ")%(y)
					elif diff <= 0:
						dup_count += 1
						print(" duplicate line %d ")%(y)

				
                                ti = []
                                last_value = t1
				current_sample = y

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
		print "miss count", miss_count, 'duplicate count ', dup_count, ' linecount', line_count
		ax1.plot(li1)
		show()


except KeyError as K:
        print "KeyError:",K
        
except OSError as E:
        print "OSError: ",E


