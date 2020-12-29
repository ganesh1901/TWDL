import os
import flatbuffers
import ViewController.Interface_Type
import ViewController.IdentifyHardwareInterface
import ViewController.Mil_1553
import ViewController.IP_RRU
import ViewController.USB_RRU
import ViewController.Transaction

from PyQt4 import QtGui, QtCore
from VarList import StyleSheet
import struct
from VarList import TWDL
import datetime
import math
import bitstruct


class NormalData:
    def __init__(self, ot, ff, twdl):
        self.ot = ot
        self.ff = ff
        self.twdl_var = twdl
        self.f1 = open('obc-mlp-uplink.txt', 'w+')
        # to control the update rate
        self.counter = 0
        self.st = StyleSheet()
        self.linearity_count = 0
        self.ground_data_downlinkfp = open("../out/Downlink.c", "w+")
        self.ttt = 0

    def getvalue(self, val, offset, scale):
        temp = (val * scale) - offset
        # print "val- %d offset- %d scale- %d result- %f" % (val, offset, scale, temp)
        return temp

    @QtCore.pyqtSlot()
    def Ground_Health_Update(self):
        try:

            buff = self.ot.ground_health.pop()
            aa = struct.unpack(">I2BHH7B6H", buff)

            map_items = self.ot.ground_twdl_map[7]

            byte_offset = self.twdl_var.ground_config_rsp_bytes_health_offset

            for i in range(len(byte_offset)):
                map_items[i].setText(str(aa[byte_offset[i]]))

        except struct.error as SE:
            print 'At Ground Health Exception ************** ', SE

    @QtCore.pyqtSlot()
    def Ground_Status_Update(self):
        try:
            buff = self.ot.ground_status.pop()
            aa = struct.unpack(">I2BHH13B13H8B", buff)

            map_items = self.ot.ground_twdl_map[8]

            byte_offset = self.twdl_var.ground_config_rsp_bytes_status_offset
            offset2 = self.twdl_var.ground_config_rsp_ui_status_offset

            for i in range(len(byte_offset)):
                map_items[offset2[i]].setText(str(aa[byte_offset[i]]))

            if aa[29] == 0xffff:
                map_items[5].setText("Port 2")
            else:
                map_items[5].setText("Port 1")

            if aa[30] == 0xff01:
                map_items[6].setText(' 1 Watt')
            elif aa[30] == 0xff02:
                map_items[6].setText(' 5 Watt')
            elif aa[30] == 0xff04:
                map_items[6].setText(' 15 Watt')
            else:
                map_items[6].setText(' PA Off')

            if aa[35] == 0xff:
                map_items[12].setText(" TRACK ")
            else:
                map_items[12].setText(" ACQ ")

            if aa[36] == 0xff:
                map_items[13].setText(" TRACK ")
            else:
                map_items[13].setText(" ACQ ")

            if aa[37] == 0xff:
                map_items[14].setText(" ON ")
            else:
                map_items[14].setText(" OFF ")

            if aa[38] == 0xff:
                map_items[15].setText(" ON ")
            else:
                map_items[15].setText(" OFF ")

            if int((aa[34] >> 4) & 0xf) == 15:
                map_items[9].setText('Locked')
            else:
                map_items[9].setText('UNLocked')

            if int(aa[34] & 0x0f) == 15:
                map_items[10].setText('Locked')
            else:
                map_items[10].setText('UNLocked')

            if int(aa[33] & 0x0f) == 15:
                map_items[11].setText(' Locked')
            else:
                map_items[11].setText('UNLocked')


        except TypeError as TE:
            print '********* STATUS ERROR ----', TE
        except struct.error as SE:
            print 'At Ground Status Exception ************** ', SE

    @QtCore.pyqtSlot()
    def show_status(self):
        try:
            length = struct.calcsize(self.twdl_var.config_rsp_fmt)
            var = bytearray(self.ot.raw_data.pop())
            # print 'raw data',  var
            # print 'length', length, '    ',len(var)

            a = struct.unpack(self.twdl_var.config_rsp_fmt, var[0:length])
            update_list = self.ot.twdl_map[9]

            offset = self.twdl_var.onboard_config_rsp_ui_offset
            len1 = len(offset)
            for i in range(len1):
                if isinstance(self.twdl_var.config_rsp_field_index[i], str):
                    key = self.twdl_var.config_rsp_field_index[i]
                    t1 = key.split('|')

                    if t1 != '':
                        # got value from the unpack data
                        val = a[int(t1[0])]
                        # MSB
                        if int(t1[1]) == 2:
                            if int(val >> 4) == 15:
                                update_list[offset[i]].setText("Locked")
                            else:
                                update_list[offset[i]].setText("UnLocked")

                        # LSB
                        else:
                            if int(val & 0xf) == 15:
                                update_list[offset[i]].setText("Locked")
                            else:
                                update_list[offset[i]].setText("UnLocked")

                else:
                    # print 'self.twdl_var.config_rsp_field_index[i] *******', self.twdl_var.config_rsp_field_index[i]
                    update_list[offset[i]].setText(str(a[self.twdl_var.config_rsp_field_index[i]]))

            if a[10] == 0xffff:
                update_list[5].setText(" Port 2 ")
            else:
                update_list[5].setText(" Port 1 ")

            if a[11] == 0xff01:
                update_list[6].setText(" 1 Watt ")
            elif a[11] == 0xff02:
                update_list[6].setText(" 5 Watt ")
            elif a[11] == 0xff03:
                update_list[6].setText(" 10 Watt ")
            elif a[11] == 0xff04:
                update_list[6].setText(" 15 Watt ")
            else:
                update_list[6].setText(" PA OFF ")

            if a[16] == 0xff:
                update_list[12].setText(' TRACK ')
            else:
                update_list[12].setText(' ACQ ')

            if a[17] == 0xff:
                update_list[13].setText(' TRACK ')
            else:
                update_list[13].setText(' ACQ ')

            if a[18] == 0xff:
                update_list[14].setText(' ON ')
            else:
                update_list[14].setText(' OFF ')

            if a[19] == 0xff:
                update_list[15].setText(' ON ')
            else:
                update_list[15].setText(' OFF ')

        except struct.error as SE:
            print '*********** show_status *******', SE
        except KeyError as tt:
            print '******* show status key value', tt, self.ot.twdl_map[9]
        except TypeError as TE:
            print 'show status--> ', TE
        except IndexError as IE:
            print 'Index', update_list, len(update_list)

    @QtCore.pyqtSlot()
    def show_health(self):
        try:
            length = struct.calcsize(self.twdl_var.config_rsp_fmt)
            var = bytearray(self.ot.raw_data.pop())

            # print 'length', length, '    ',len(var)
            a = struct.unpack(self.twdl_var.health_rsp_fmt, var[0:length])

            update_list = self.ot.twdl_map[8]
            len1 = len(update_list)
            for i in range(len1):
                if isinstance(self.twdl_var.health_rsp_field_index[i], str):
                    key = self.twdl_var.health_rsp_field_index[i]
                    t1 = key.split('|')

                    if t1 != '':
                        # got value from the unpack data
                        val = a[int(t1[0])]
                        # MSB
                        if int(t1[1]) == 2:
                            if int(val >> 4) == 15:
                                update_list[i].setText("Locked")
                            else:
                                update_list[i].setText("UnLocked")

                        # LSB
                        else:
                            if int(val & 0xf) == 15:
                                update_list[i].setText("Locked")
                            else:
                                update_list[i].setText("UnLocked")
                else:
                    update_list[i].setText(str(a[self.twdl_var.config_rsp_field_index[i]]))

        except struct.error as SE:
            print '*********** show_health *******', SE
        except KeyError as tt:
            print '******* show health key value', tt, self.ot.twdl_map[8]
        except TypeError as TE:
            print 'show health--> ', TE

    @QtCore.pyqtSlot()
    def Missile_Data_log(self):

        # print 'missile data log'
        try:
            buf = bytearray(self.ot.twdl_raw_data.pop())
            b1 = struct.unpack("2H", buf[0:4])
            if b1[1] == 0x5455:
                li = []
                # print 'Packet C'
                # bytes-6
                b2 = bitstruct.unpack("u8u8u2u6u8u16", buf[4:10][::-1])
                for i in b2[::-1]:
                    li.append(i)

                # Missile position 7 bytes
                b4 = bitstruct.unpack("u18u19u19", buf[10:17][::-1])
                li.append(self.getvalue(b4[2], 150000, 1))
                li.append(self.getvalue(b4[1], 150000, 1))
                li.append(self.getvalue(b4[0], 60000, 1))

                # Missile Velocity 5 bytes
                b5 = bitstruct.unpack("u13u13u14", buf[17:22][::-1])
                li.append(self.getvalue(b5[2], 1500, 1))
                li.append(self.getvalue(b5[1], 1500, 1))
                li.append(self.getvalue(b5[0], 1500, 1))

                # Missile Acceleration 4 bytes
                b4 = bitstruct.unpack("u10u11u11", buf[22:26][::-1])
                li.append(self.getvalue(b4[2], 400, 1))
                li.append(self.getvalue(b4[1], 400, 1))
                li.append(self.getvalue(b4[0], 400, 1))

                # Body rates  4 bytes
                b4 = bitstruct.unpack("u10u11u11", buf[26:30][::-1])
                li.append(self.getvalue(b4[2], 400, 1))
                li.append(self.getvalue(b4[1], 400, 1))
                li.append(self.getvalue(b4[0], 400, 1))

                # Euler rates 4 bytes
                b4 = bitstruct.unpack("u10u11u11", buf[30:34][::-1])
                li.append(self.getvalue(b4[2], 180, 1))
                li.append(self.getvalue(b4[1], 90, 1))
                li.append(self.getvalue(b4[0], 90, 1))

                # control degrees 3 bytes
                b4 = bitstruct.unpack("u6u6u6u6", buf[34:37][::-1])
                li.append(self.getvalue(b4[3], 30, 1))
                li.append(self.getvalue(b4[2], 30, 1))
                li.append(self.getvalue(b4[1], 30, 1))
                li.append(self.getvalue(b4[0], 30, 1))

                b4 = struct.unpack("B", buf[37:38])
                li.append(b4[0] & 0xf)
                li.append(b4[0] >> 4 & 0xf)

                b4 = struct.unpack("H", buf[38:40])
                li.append(b4[0])
                # print li

                val1 = self.ot.ground_map[2]
                for ii in range(len(val1)):
                    val1[ii].setText(str(li[ii]))

            # self.ot.plot_data[2][1].append(li[4])
            # self.ot.pktB_seq[2].setData(self.ot.plot_data[2][1])

        except struct.error as SE:
            print '*********** Missile_Data_log *******', SE

    @QtCore.pyqtSlot()
    def Missile_Data_log1(self):
        try:
            b12 = bytearray(self.ot.twdl_raw_data1.pop())
            for i in range(4, len(b12)):
                self.f1.write("%02X " % (b12[i]))
            self.f1.write('\n')
            self.f1.flush()

            b2 = struct.unpack("2HIBB", b12[0:10])
            # uplink packet A
            if b2[4] == 1:
                li = []

                b2 = struct.unpack("IBBB", b12[4:11])
                li.append(b2[0])
                li.append(b2[1])

                b2 = bitstruct.unpack("u2u6", b12[11:12])
                b2 = b2[::-1]
                for i in range(len(b2)):
                    li.append(b2[i])

                b2 = struct.unpack("H", b12[12:14])
                li.append(b2[0])

                self.ot.uplink_obc_cseq[0] = b2[0]

                # print "Sequence Nu", b2[0], self.ot.uplink_obc_pseq[0]
                # for the first packet
                if (self.ot.uplink_obc_count[0] == 0):
                    self.ot.uplink_obc_pseq[0] = self.ot.uplink_obc_cseq[0]

                else:
                    if (self.ot.uplink_obc_pseq[0] < self.ot.uplink_obc_cseq[0] and self.ot.uplink_obc_pseq[0] + 1 !=
                            self.ot.uplink_obc_cseq[0]):
                        self.ot.uplink_obc_miss[0] += self.ot.uplink_obc_cseq[0] - self.ot.uplink_obc_pseq[0] - 1
                        print "Current Sequence No", b2[0], '    Pre Sequence No', self.ot.uplink_obc_pseq[0]

                    self.ot.uplink_obc_pseq[0] = self.ot.uplink_obc_cseq[0]

                self.ot.uplink_obc_count[0] += 1

                # 7 bytes position
                b4 = bitstruct.unpack("u18u19u19", b12[14:21][::-1])
                x = self.getvalue(b4[2], 150000, 1)
                y = self.getvalue(b4[1], 150000, 1)
                z = self.getvalue(b4[0], 60000, 1)
                li.append(x)
                li.append(y)
                li.append(z)
                if self.ot.uplink:

                    self.ot.plot_data[1][0].append(x)
                    self.ot.plot_data[1][1].append(y)
                    self.ot.plot_data[1][2].append(z)

                    self.ot.params[0].setData(self.ot.x_plot_data[0], self.ot.plot_data[1][0])
                    self.ot.params[1].setData(self.ot.x_plot_data[0], self.ot.plot_data[1][1])
                    self.ot.params[2].setData(self.ot.x_plot_data[0], self.ot.plot_data[1][2])


                # 5 bytes velocity
                b5 = bitstruct.unpack("u13u13u14", b12[21:26][::-1])
                li.append(self.getvalue(b5[2], 1500, 1))
                li.append(self.getvalue(b5[1], 1500, 1))
                li.append(self.getvalue(b5[0], 1500, 1))

                # 3 bytes acceleration
                b4 = bitstruct.unpack("u8u8u8", b12[26:29][::-1])
                li.append(self.getvalue(b4[2], 100, 1))
                li.append(self.getvalue(b4[1], 100, 1))
                li.append(self.getvalue(b4[0], 100, 1))

                # 4 bytes postion variance
                b4 = bitstruct.unpack("u10u11u11", b12[29:33][::-1])
                li.append(self.getvalue(b4[2], 0, 1))
                li.append(self.getvalue(b4[1], 0, 1))
                li.append(self.getvalue(b4[0], 0, 1))

                # 3 bytes velocity variance
                b4 = bitstruct.unpack("u8u8u8", b12[33:36][::-1])
                li.append(self.getvalue(b4[2], 0, 1))
                li.append(self.getvalue(b4[1], 0, 1))
                li.append(self.getvalue(b4[0], 0, 1))

                b4 = struct.unpack("BBH", b12[36:40])
                for i in b4:
                    li.append(i)
                # print li
                tt = self.ot.onboard_map[0]
                for i in range(len(tt) - 1):
                    tt[i + 1].setText(str(li[i]))

            # self.ot.plot_data[0][1].append(li[4])
            # self.ot.pktB_seq[0].setData(self.ot.plot_data[0][1])


            elif b2[4] == 2:
                # print 'in else part sub--4'
                li = []

                b2 = struct.unpack("IBBB", b12[4:11])
                li.append(b2[0])
                li.append(b2[1])

                b2 = bitstruct.unpack("u2u6", b12[11:12])
                b2 = b2[::-1]
                for i in range(len(b2)):
                    li.append(b2[i])

                b2 = struct.unpack("H", b12[12:14])
                li.append(b2[0])

                self.ot.uplink_obc_cseq[1] = b2[0]

                # print "Sequence Nu", b2[0], self.ot.uplink_obc_pseq[0]
                # for the first packet
                if (self.ot.uplink_obc_count[1] == 0):
                    self.ot.uplink_obc_pseq[1] = self.ot.uplink_obc_cseq[1]

                else:
                    if (self.ot.uplink_obc_pseq[1] < self.ot.uplink_obc_cseq[1] and self.ot.uplink_obc_pseq[1] + 1 !=
                            self.ot.uplink_obc_cseq[0]):
                        self.ot.uplink_obc_miss[1] += self.ot.uplink_obc_cseq[1] - self.ot.uplink_obc_pseq[1] - 1
                        print "Current Sequence No", b2[0], '    Pre Sequence No', self.ot.uplink_obc_pseq[1]

                    self.ot.uplink_obc_pseq[1] = self.ot.uplink_obc_cseq[1]

                self.ot.uplink_obc_count[1] += 1

                # 7 bytes Target position
                b4 = bitstruct.unpack("u18u19u19", b12[14:21][::-1])
                li.append(self.getvalue(b4[2], 150000, 1))
                li.append(self.getvalue(b4[1], 150000, 1))
                li.append(self.getvalue(b4[0], 60000, 1))

                # 5 bytes Target velocity
                b5 = bitstruct.unpack("u13u13u14", b12[21:26][::-1])
                li.append(self.getvalue(b5[2], 1500, 1))
                li.append(self.getvalue(b5[1], 1500, 1))
                li.append(self.getvalue(b5[0], 1500, 1))

                # 7 bytes Missile position
                b4 = bitstruct.unpack("u18u19u19", b12[26:33][::-1])
                li.append(self.getvalue(b4[2], 150000, 1))
                li.append(self.getvalue(b4[1], 150000, 1))
                li.append(self.getvalue(b4[0], 60000, 1))

                # 5 bytes postion variance
                b4 = bitstruct.unpack("u13u13u14", b12[33:38][::-1])
                li.append(self.getvalue(b4[2], 1500, 1))
                li.append(self.getvalue(b4[1], 1500, 1))
                li.append(self.getvalue(b4[0], 1500, 1))

                b4 = struct.unpack("H", b12[38:40])
                for i in b4:
                    li.append(i)
                # print li
                tt = self.ot.onboard_map[1]
                for i in range(len(tt) - 1):
                    tt[i + 1].setText(str(li[i]))

            # self.ot.plot_data[1][1].append(li[4])
            # self.ot.pktB_seq[1].setData(self.ot.plot_data[1][1])

        except struct.error as se:
            print '******* Missile_Data_log1 ******', se

    @QtCore.pyqtSlot()
    def Show_Missile_Version(self):
        try:

            length = struct.calcsize(self.twdl_var.version_rsp_fmt)
            var = bytearray(self.ot.raw_data.pop())

            # print 'length', length, '    ',len(var)
            a = struct.unpack(self.twdl_var.version_rsp_fmt, var[0:length])

            update_list = self.ot.twdl_map[0xc]

            # H H I 2B H 6B 23H
            # 0-H 1-H 2-I 3-42B  5H 3H 23H
            if self.twdl_var.obc_flag == 1:
                update_list[0].setText(str(a[3]))
                update_list[1].setText("%d.%d.%d" % (a[7], a[6] >> 4, a[6] & 0x0f))
                update_list[2].setText("%d-%d-%d" % ((a[8] & 0x0ff0) >> 4, a[8] & 0x000f, a[9]))

                print 'version data ', a, " %02X" % ((a[8] & 0x0ff0) >> 4)
            QtGui.QApplication.instance().processEvents()
            self.ot.getScreenShot('../out/ONBOARD_DL_Version')

        except struct.error as SE:
            print '*********** missile twdl version *******', SE
        except KeyError as tt:
            print '******* missile twdl version', tt, self.ot.twdl_map[8]
        except TypeError as TE:
            print 'missile twdl version--> ', TE

    @QtCore.pyqtSlot()
    def dataupdate_ground_packetC(self):
        b1 = self.ot.downlink_data.pop()
        if b1:
            # logging to file
            self.ground_data_downlinkfp.write(self.ot.GetDateTime())
            self.ground_data_downlinkfp.write('\t')

            b2 = bytearray(b1)
            for i in range(len(b2)):
                self.ground_data_downlinkfp.write("%02X " % (b2[i]))
            self.ground_data_downlinkfp.write('\n')

            li = []
            b2 = struct.unpack(">IBBHH", b1[0:10])
            for i in b2:
                li.append(i)

            if b2[2] == 0x33:
                self.ot.ground_dnlink_count += 1
                self.ot.ground_dnlink_cseq = b2[3]
                #print self.ot.ground_dnlink_cseq, self.ot.ground_dnlink_pseq
                if self.ot.ground_dnlink_cseq != self.ot.ground_dnlink_pseq + 1:
                    self.ot.ground_dnlink_miss += 1
                else:
                    self.ot.ground_dnlink_pseq = self.ot.ground_dnlink_cseq

                # bytes-6
                b2 = bitstruct.unpack("u8u8u2u6u8u16", b1[10:16][::-1])
                b2 = b2[::-1]
                for i in range(len(b2)):
                    li.append(b2[i])

                # print b2[i], '***', i
                if b2[i] == 3:
                    # Missile position 7 bytes
                    b4 = bitstruct.unpack("u18u19u19", b1[16:23][::-1])
                    li.append(self.getvalue(b4[2], 150000, 1))
                    li.append(self.getvalue(b4[1], 150000, 1))
                    li.append(self.getvalue(b4[0], 60000, 1))

                    # Missile Velocity 5 bytes
                    b5 = bitstruct.unpack("u13u13u14", b1[23:28][::-1])
                    li.append(self.getvalue(b5[2], 1500, 1))
                    li.append(self.getvalue(b5[1], 1500, 1))
                    li.append(self.getvalue(b5[0], 1500, 1))

                    # Missile Acceleration 4 bytes
                    b4 = bitstruct.unpack("u10u11u11", b1[28:32][::-1])
                    li.append(self.getvalue(b4[2], 400, 1))
                    li.append(self.getvalue(b4[1], 400, 1))
                    li.append(self.getvalue(b4[0], 400, 1))

                    # Body rates  4 bytes
                    b4 = bitstruct.unpack("u10u11u11", b1[32:36][::-1])
                    li.append(self.getvalue(b4[2], 400, 1))
                    li.append(self.getvalue(b4[1], 400, 1))
                    li.append(self.getvalue(b4[0], 400, 1))

                    # Euler rates 4 bytes
                    b4 = bitstruct.unpack("u10u11u11", b1[36:40][::-1])
                    li.append(self.getvalue(b4[2], 180, 1))
                    li.append(self.getvalue(b4[1], 90, 1))
                    li.append(self.getvalue(b4[0], 90, 1))

                    # control degrees 3 bytes
                    b4 = bitstruct.unpack("u6u6u6u6", b1[40:43][::-1])
                    li.append(self.getvalue(b4[3], 30, 1))
                    li.append(self.getvalue(b4[2], 30, 1))
                    li.append(self.getvalue(b4[1], 30, 1))
                    li.append(self.getvalue(b4[0], 30, 1))

                    b4 = struct.unpack("B", b1[43:44])
                    li.append(b4[0] & 0xf)
                    li.append(b4[0] >> 4 & 0xf)

                    b4 = struct.unpack("H", b1[44:46])
                    li.append(b4[0])
                    # print li

                    val1 = self.ot.onboard_map[2]
                    for ii in range(len(val1)):
                        val1[ii].setText(str(li[5 + ii]))
            # self.ot.downlink_counts.setText(
            #     "Packet C Received: %d MissCount: %d" % (
            #         self.ot.ground_dnlink_count, self.ot.ground_dnlink_miss))

        # self.ot.plot_data[2][0].append(li[9])
        # self.ot.pktA_seq[2].setData(self.ot.plot_data[2][0])

    @QtCore.pyqtSlot()
    def ground_version(self):
        try:
            length = struct.calcsize(self.twdl_var.ground_version_rsp_fmt)
            var = bytearray(self.ot.raw_data.pop())
            a = struct.unpack(self.twdl_var.ground_version_rsp_fmt, var)

            update_list = self.ot.ground_twdl_map[0xe]
            update_list[0].setText("%08x,%d" % (a[5], a[11]))
            update_list[1].setText("%d.%d" % (a[6], a[7]))
            update_list[2].setText("%d-%d-%d" % (a[8], a[9], a[10]))
            QtGui.QApplication.instance().processEvents()
            self.ot.getScreenShot('../out/GROUND_DL_Version')
        except:
            pass

class FlatcOperations(QtCore.QObject):
    health_signal = QtCore.pyqtSignal()
    status_signal = QtCore.pyqtSignal()
    obc_downlink_data = QtCore.pyqtSignal()
    twdl_missile_version_signal = QtCore.pyqtSignal()
    ground_health_signal = QtCore.pyqtSignal()
    ground_status_signal = QtCore.pyqtSignal()
    downlink_data_signal = QtCore.pyqtSignal()
    uplink_data_signal = QtCore.pyqtSignal()
    ground_version_signal = QtCore.pyqtSignal()
    timer_packetA = QtCore.QTimer()

    def __init__(self, ot, ff, twdl):
        QtCore.QObject.__init__(self)
        self.ot = ot
        self.ff = ff
        self.twdl = twdl
        self.st = StyleSheet()

        self.nor = NormalData(self.ot, self.ff, self.twdl)
        self.status_signal.connect(self.nor.show_status)
        self.health_signal.connect(self.nor.show_health)
        self.obc_downlink_data.connect(self.nor.Missile_Data_log)
        self.twdl_missile_version_signal.connect(self.nor.Show_Missile_Version)

        self.ground_health_signal.connect(self.nor.Ground_Health_Update)
        self.ground_status_signal.connect(self.nor.Ground_Status_Update)

        self.downlink_data_signal.connect(self.nor.dataupdate_ground_packetC)
        self.uplink_data_signal.connect(self.nor.Missile_Data_log1)

        self.ground_version_signal.connect(self.nor.ground_version)
        self.timer_packetA.setInterval(100)
        self.timer_packetA.timeout.connect(self.SendTargetData)

        self.surv_count = 0
        self.ins_count = 0
        self.ins_count1 = 0
        self.counter = 0
        self.counter1 = 0
        self.temp = 0
        self.temp1 = 0
        self.ta1 = 0
        self.fa = open('../out/Target.txt', "w+")
        self.fa_bin = open('../out/Target.bin', "w+")
        self.tt = 0

    def SendTargetData(self):
        self.ta1 += 1
        li = self.ot.ground_map[0]

        if self.counter1 == 65535:
            self.counter1 = 0
        else:
            self.counter1 += 1
            li[0].setText(str(self.ta1))

        # counter = int(li[0].text())

        header = [1234, 1, 1, self.ta1]
        metadata = ""

        pack_data = struct.pack(">IBBH", *header)
        metadata += pack_data

        if self.counter1 > 5000:
            self.counter1 = 0

        b1 = [self.counter1 * 4000, 1, 1, 2, 3, self.ta1]
        b11 = struct.pack("=I2B2BH", *b1)
        metadata += b11
        li[5].setText(str(self.ta1))

        lx1 = []
        for i in range(len(self.ot.Tgt_init_pos_LCI)):
            lx1.append(self.ot.Tgt_init_pos_LCI[i] + self.ot.Tgt_init_vel_LCI[i] * self.ta1 * 0.1)
            li[6+i].setText(" %6.2f " % (lx1[i]))
            #print lx1[i]

        x = self.ot.setvalue(lx1[0],  150000, 1)
        y = self.ot.setvalue(lx1[1], 150000, 1)
        z = self.ot.setvalue(lx1[2], 60000, 1)

        b2 = bitstruct.pack("u18u19u19", *[z, y, x])
        metadata += b2[::-1]

        x = self.ot.setvalue(float(li[9].text()), 1500, 1)
        y = self.ot.setvalue(float(li[10].text()), 1500, 1)
        z = self.ot.setvalue(float(li[11].text()), 1500, 1)

        b2 = bitstruct.pack("u13u13u14", *[z, y, x])
        metadata += b2[::-1]

        x = self.ot.setvalue(float(li[12].text()), 100, 1)
        y = self.ot.setvalue(float(li[13].text()), 100, 1)
        z = self.ot.setvalue(float(li[14].text()), 100, 1)

        b2 = bitstruct.pack("u8u8u8", *[z, y, x])
        metadata += b2[::-1]

        x = self.ot.setvalue(float(li[15].text()), 0, 1)
        y = self.ot.setvalue(float(li[16].text()), 0, 1)
        z = self.ot.setvalue(float(li[17].text()), 0, 1)

        b2 = bitstruct.pack("u10u11u11", *[z, y, x])
        metadata += b2[::-1]

        x = self.ot.setvalue(float(li[18].text()), 0, 1)
        y = self.ot.setvalue(float(li[19].text()), 0, 1)
        z = self.ot.setvalue(float(li[20].text()), 0, 1)

        b2 = bitstruct.pack("u8u8u8", *[z, y, x])
        metadata += b2[::-1]

        b11 = struct.pack("BBH", *[int(li[21].text()), int(li[22].text()), 0x1234])
        metadata += b11

        checksum = self.ot.crc16(metadata)
        aa = struct.pack('>H', checksum)

        li1 = metadata[0:8] + aa + metadata[8:]


        li12 = bytearray(li1)

        # print '************', len(li12)
        self.fa.write(self.ot.GetDateTime())
        self.fa.write('\t')
        for i1 in range(len(li12)):
            self.fa.write("%02x " % (li12[i1]))
        self.fa.write('\n')
        self.fa.flush()

        self.fa_bin.write(li1)

        self.ff.udp_conn.sendto(li1, (self.ff.serverip, 6510))

        if self.ta1 % self.ot.samples_seeker == 1:
            self.tt += 1
            self.ot.pyqtgraph[0].setXRange(self.ot.samples_seeker * 0.1 * (self.tt - 1),
                                                             (self.ot.samples_seeker * 0.1) * self.tt)
            self.ot.pyqtgraph[1].setXRange(self.ot.samples_seeker * 0.1 * (self.tt - 1),
                                           (self.ot.samples_seeker * 0.1) * self.tt)
            self.ot.pyqtgraph[2].setXRange(self.ot.samples_seeker * 0.1 * (self.tt - 1),
                                           (self.ot.samples_seeker * 0.1) * self.tt)

        self.ot.plot_data[0][0].append(lx1[0])
        self.ot.plot_data[0][1].append(lx1[1])
        self.ot.plot_data[0][2].append(lx1[2])

        self.ot.x_plot_data[0].append(self.ta1 * 0.1)
        self.ot.params[0].setData(self.ot.x_plot_data[0], self.ot.plot_data[0][0])
        self.ot.params[1].setData(self.ot.x_plot_data[0], self.ot.plot_data[0][1])
        self.ot.params[2].setData(self.ot.x_plot_data[0], self.ot.plot_data[0][2])




        # self.ot.pktA_seq[0].setData(self.ot.plot_data[0][0])
        # if self.ta1 % 10 == 0:
        #     self.ot.uplinkA_count.setText(
        #         "Packet A Sent: %d Received: %d MissCount: %d" % (
        #         self.ta1, self.ot.uplink_obc_count[0], self.ot.uplink_obc_miss[0]))

    def PrepareObject(self, interface_type, payload):

        print 'faltc ', interface_type, '     ', payload
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
                print payload[1], 'size', payload[1]

            builder.Finish(orc)
            buf = builder.Output()

            print '^^^^^^^^^^^^^^^^^^^^size is', len(buf)
            os.write(self.ff.cmd_fd, buf)


        except TypeError as type_err:
            print  '*****', type_err

    def DecodeObject(self, buff):
        try:
            # print 'size at decode object  --', len(buff)
            Transaction = ViewController.Transaction.Transaction.GetRootAsTransaction(buff, 0)

            # print "******************************Received from CPP Side***********************"
            # print "QueryonInterface==", Transaction.QueryOnInterface()

            if Transaction.QueryOnInterfaceType() == ViewController.IdentifyHardwareInterface.IdentifyHardwareInterface().IP_RRU:
                union_ip_rru = ViewController.IP_RRU.IP_RRU()
                union_ip_rru.Init(Transaction.QueryOnInterface().Bytes, Transaction.QueryOnInterface().Pos)

                if union_ip_rru.IType() == ViewController.Interface_Type.Interface_Type().DIP:

                    key1 = str(self.ot.slot_map[union_ip_rru.Slot()] + '-' + str(union_ip_rru.Port()) + '-' + str(
                        union_ip_rru.Channel()))
                    print 'key hhhhh', key1
                    if union_ip_rru.Value():
                        self.ot.dip_map[key1][1].setPixmap(QtGui.QPixmap('../res/green.png'))
                    else:
                        self.ot.dip_map[key1][1].setPixmap(QtGui.QPixmap('../res/red.png'))

                elif union_ip_rru.IType() == ViewController.Interface_Type.Interface_Type().DOP:
                    pass

                else:
                    key1 = str(self.ot.slot_map[union_ip_rru.Slot()] + '-' + str(union_ip_rru.Port()) + '-' + str(
                        union_ip_rru.Channel()))
                    print 'adc key ', key1
                    self.ot.adc_map[key1][1].setText(str(union_ip_rru.Value2())[0:9])
                    QtGui.QApplication.instance().processEvents()

            elif Transaction.QueryOnInterfaceType() == ViewController.IdentifyHardwareInterface.IdentifyHardwareInterface().Mil_1553:

                try:
                    union_mil1553 = ViewController.Mil_1553.Mil_1553()
                    union_mil1553.Init(Transaction.QueryOnInterface().Bytes, Transaction.QueryOnInterface().Pos)

                    self.ins_count1 += 1

                    # print 'subaddress', union_mil1553.Subaddress()
                    temp_list = []
                    for i in range(union_mil1553.PayloadLength()):
                        temp_list.append(union_mil1553.Payload(i))

                    if union_mil1553.Subaddress() == self.twdl.health_data_subadd:
                        self.ot.raw_data.append(temp_list)
                        self.health_signal.emit()
                    elif union_mil1553.Subaddress() == self.twdl.status_data_subadd:
                        self.ot.raw_data.append(temp_list)
                        self.status_signal.emit()
                    elif union_mil1553.Subaddress() == self.twdl.version_subaddr:
                        self.ot.raw_data.append(temp_list)
                        self.twdl_missile_version_signal.emit()
                    elif union_mil1553.Subaddress() == 14:
                        self.ot.twdl_raw_data.append(temp_list)
                        self.obc_downlink_data.emit()

                    elif union_mil1553.Subaddress() == 4:
                        self.ot.twdl_raw_data1.append(temp_list)
                        self.uplink_data_signal.emit()

                    # Normal response
                    else:
                        # print '###################   Normal data   ******************** Subaddress()', union_mil1553.Subaddress()
                        self.ot.raw_data.append(temp_list)
                        self.normal_signal.emit()

                except KeyError as tt:
                    print '******* key value', tt, self.ot.mil_rspmap[0x1408][union_mil1553.Subaddress()]

                except struct.error as se:
                    print '*************** struct ', se, len(bytearray(temp_list))

        except TypeError as TE:
            print '*/*/*/*/*/*', TE

        except AttributeError as AE:
            print '/*/*/*/*/*/', AE

        except struct.error as SE:
            print 'struct exception ****************', SE

    def DecodeUdpPacket(self, buff):
        # print 'buffer ', len(buff), '--------'

        # health
        if len(buff) == 29:
            self.ot.ground_health.append(buff)
            self.ground_health_signal.emit()

        # Status
        elif len(buff) == 57:
            self.ot.ground_status.append(buff)
            self.ground_status_signal.emit()

        # Dowlink C,D&E
        elif len(buff) == 46:
            self.ot.downlink_data.append(buff)
            # print 'GDLC DATA'
            self.downlink_data_signal.emit()

        # Ground Version
        elif len(buff) == 24:
            self.ot.raw_data.append(buff)
            self.ground_version_signal.emit()
            print 'lenof buff', len(buff)

