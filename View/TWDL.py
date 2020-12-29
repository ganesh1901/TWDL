from PyQt4 import QtGui, QtCore
from VarList import TWDL
from VarList import StyleSheet
from Component import Component
import struct
import socket
import time
import pyqtgraph as pg
pg.setConfigOptions(antialias=True)
import bitstruct


class TWDLWindow:
    packetA_timer = QtCore.QTimer()
    packetA_timer.setInterval(100)
    packetB_timer = QtCore.QTimer()
    packetB_timer.setInterval(100)

    counter1 = 0
    counter2 = 0

    def __init__(self, widget, ot, flatc, ff, twdl):
        self.ot = ot
        self.flatc = flatc
        self.ff = ff
        self.twdl_var = twdl
        self.widget = widget
        self.st = StyleSheet()
        self.comp = Component()
        self.fa = open('../out/packetA.txt', "w+")
        self.fb = open('../out/packetB.txt', "w+")
        layout = QtGui.QHBoxLayout(widget)


        self.ot.packet_A_timer = self.packetA_timer
        self.ot.packet_B_timer = self.packetB_timer

        tabwidget = QtGui.QTabWidget()
        tabwidget.setFixedHeight(widget.height() * 0.98)
        tabwidget.setFixedWidth(widget.width() - 20)
        tabwidget.setStyleSheet(self.st.tabWidget1)

        layout.addWidget(tabwidget)

        tab_list_name = ["Settings", "Data"]
        tab_list = [0, 1]
        for i in range(2):
            tab_list[i] = QtGui.QWidget()
            tab_list[i].setFixedWidth(tabwidget.width() * 0.98)
            tab_list[i].setFixedHeight(tabwidget.height() * 0.97)
            tabwidget.addTab(tab_list[i], tab_list_name[i])

        layout1 = QtGui.QHBoxLayout(tab_list[0])
        self.missile_twdl_gbox = self.comp.getGroupBox("Missile", tab_list[0].width() * 0.49,
                                                       tab_list[0].height() * .97)
        self.ground_twdl_gbox = self.comp.getGroupBox("Ground", tab_list[0].width() * 0.49, tab_list[0].height() * 0.97)

        layout1.addWidget(self.missile_twdl_gbox)
        layout1.addWidget(self.ground_twdl_gbox)

        self.TWDL_UI(self.missile_twdl_gbox, "MIL1553")
        self.TWDL_UI(self.ground_twdl_gbox, "Ethernet")

        layout2 = QtGui.QHBoxLayout(tab_list[1])
        for i in range(len(self.twdl_var.data_items)):
            gbox = self.comp.getGroupBox(self.twdl_var.data_items[i], tab_list[1].width() * 0.25, tab_list[1].height() * 0.95)
            gbox_l = QtGui.QVBoxLayout(gbox)
            li = []
            li1 = []
            vbox = QtGui.QVBoxLayout()
            for ii in range(len(self.twdl_var.data_items_list[i])):
                hbox = QtGui.QHBoxLayout()
                label = self.comp.getLabel(self.twdl_var.data_items_list[i][ii], 200, -1)

                #missile_log_gbox = self.comp.getGroupBox("Missile", tab_list[1].width() * 0.49, tab_list[1].height() * .92)
                #ground_log_gbox = self.comp.getGroupBox("Ground", tab_list[1].width() * 0.49, tab_list[1].height() * 0.92)

                if i != 2:
                    line1 = self.comp.getLineEdit(True, 120, -1)
                    line1.setText(str(self.twdl_var.pkt_items_default[i][ii]))
                else:
                    line1 = self.comp.getLineEdit(False, 95, -1)
                li.append(line1)

                line2 = self.comp.getLineEdit(False, 95, -1)
                if i != 2:
                    #line2.setText(str(self.twdl_var.pktB_items_default[ii]))
                    pass
                li1.append(line2)
                hbox.addWidget(label)
                hbox.addWidget(line1)
                hbox.addWidget(line2)
                vbox.addLayout(hbox)
            if i != 2:
                hbox = QtGui.QHBoxLayout()
                button = self.comp.getButton("Send Once", 120, -1)
                button1 = self.comp.getButton("Periodically", 130, -1)
                button2 = self.comp.getButton("Stop", 120, -1)
                QtCore.QObject.connect(button, QtCore.SIGNAL("clicked()"), lambda a=button, b=i, c=0:self.PacketSend(a, b, c))
                QtCore.QObject.connect(button1, QtCore.SIGNAL("clicked()"), lambda a=button1, b=i, c=1: self.PacketSend(a, b, c))
                QtCore.QObject.connect(button2, QtCore.SIGNAL("clicked()"), lambda a=button, b=button1, c=i, d=0: self.PacketStop(a, b, c, d))

                hbox.addWidget(button)
                hbox.addWidget(button1)
                hbox.addWidget(button2)

                vbox.addLayout(hbox)
            self.ot.ground_map[i] = li
            self.ot.onboard_map[i] = li1
            gbox_l.addLayout(vbox)
            layout2.addWidget(gbox)

        plot_area = self.comp.getFrame("Sunken", tab_list[1].width() * 0.24, tab_list[1].height() * 0.95)
        plot_area_l = QtGui.QVBoxLayout(plot_area)

        # gbox1 = self.comp.getGroupBox("Status", plot_area.width() * 0.95, plot_area.height() * 0.25)
        # gbox1_l = QtGui.QVBoxLayout(gbox1)
        #
        # li = []
        # for i in range(3):
        #     label = self.comp.getLabel("", gbox1.width(), -1)
        #     gbox1_l.addWidget(label)
        #     li.append(label)
        #
        # self.ot.uplinkA_count = li[0]
        # self.ot.uplinkB_count = li[1]
        # self.ot.downlink_counts =li[2]

        #
        # vbox = QtGui.QVBoxLayout()
        # label = self.comp.getLabel("pakcet A", 100, 40)
        # vbox.addWidget(label)
        #
        # hbox1 = QtGui.QHBoxLayout()
        # label1 = self.comp.getLabel("Sent", 50, 20)
        # line = self.comp.getLineEdit(False, 50, 20)
        # self.ot.uplinkA_counts.append(line)
        # hbox1.addWidget(label1)
        # hbox1.addWidget(line)
        # vbox.addLayout(hbox1)
        #
        # hbox1 = QtGui.QHBoxLayout()
        # label1 = self.comp.getLabel("Received", 50, 20)
        # line = self.comp.getLineEdit(False, 50, 20)
        # self.ot.uplinkA_counts.append(line)
        # hbox1.addWidget(label1)
        # hbox1.addWidget(line)
        # vbox.addLayout(hbox1)
        #
        # gbox1_l.addLayout(vbox)


        self.CreateGraph(plot_area_l, plot_area, 0, "X")
        self.CreateGraph(plot_area_l, plot_area, 1, "Y")
        self.CreateGraph(plot_area_l, plot_area, 2, "Z")

        #self.CreateGraph(plot_area_l, plot_area, 0)
        #self.CreateGraph(plot_area_l, plot_area, 1)
        #self.CreateGraph(plot_area_l, plot_area, 2)
        #self.CreateGraph(plot_area_l, plot_area, 3)

        # plot_area_l.addWidget(gbox1)
        layout2.addWidget(plot_area)

        self.packetA_timer.timeout.connect(self.sendPacketA)
        self.packetB_timer.timeout.connect(self.sendPacketB)



    def sendPacketA(self):
        li = self.ot.ground_map[0]

        if self.counter1 == 65535:
            self.counter1 = 0
        else:
            self.counter1 += 1
            li[0].setText(str(self.counter1))

        #counter = int(li[0].text())

        header = [1234, 1, 1, self.counter1]
        metadata = ""

        pack_data = struct.pack(">IBBH", *header)
        metadata += pack_data


        b1 = [12345678, 1, 1, 2, 3, self.counter1]
        b11 = struct.pack("=I2B2BH", *b1)
        metadata += b11
        li[5].setText(str(self.counter1))

        x = self.ot.setvalue(float(li[6].text()), 150000, 1)
        y = self.ot.setvalue(float(li[7].text()), 150000, 1)
        z = self.ot.setvalue(float(li[8].text()), 60000, 1)

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



        checksum = self.twdl_var.crc16(metadata)
        aa = struct.pack('>H', checksum)

        li1 = metadata[0:8] + aa + metadata[8:]

        self.twdl_var.pktA_log.write(li1)
        li12 = bytearray(li1)

        '''
        ll = [0x3210, 0x5678, 0x0101, 0x1a1b, 0x2a2b, 0x3a3b, 0x4a4b, 0x5a5b, 0x6a6b, 0x7a7b, 0x8a8b, 0x9a9b, 0x1c1d, 0x2c2d, 0x3c3d, 0x4c4d, 0x5c5d, 0x6c6d]
        # print '************', len(li12)


        bb = struct.pack("18H", *ll)
        metadata += bb
        checksum = self.twdl_var.crc16(metadata)
        aa = struct.pack('>H', checksum)

        li1 = metadata[0:8] + aa + metadata[8:]
    
        self.twdl_var.pktA_log.write(li1)
        '''

        self.fa.write(self.ot.GetDateTime())
        self.fa.write('\t')
        for i1 in range(len(li12)):
            self.fa.write("%04x " % (li12[i1]))
        self.fa.write('\n')
        self.fa.flush()

        self.ff.udp_conn.sendto(li1, (self.ff.serverip, 6510))

        # self.ot.plot_data[0][0].append(self.counter1)
        # self.ot.pktA_seq[0].setData(self.ot.plot_data[0][0])
        # if self.counter1 % 10 == 0:
        #     self.ot.uplinkA_count.setText(
        #         "Packet A Sent: %d Received: %d MissCount: %d" % (self.counter1, self.ot.uplink_obc_count[0], self.ot.uplink_obc_miss[0]))


    def sendPacketB(self):
        li = self.ot.ground_map[1]

        if self.counter2 == 65535:
            self.counter2 = 0
        else:
            self.counter2 += 1
            li[0].setText(str(self.counter2))

        # counter = int(li[0].text())

        header = [1234, 1, 2, self.counter2]
        metadata = ""

        pack_data = struct.pack(">IBBH", *header)
        metadata += pack_data

        # print struct.unpack(">IBBH", metadata)

        b1 = [12345678, 1, 2, 2, 3, self.counter2]
        b11 = struct.pack("=I2B2BH", *b1)
        metadata += b11
        li[5].setText(str(self.counter2))

        x = self.ot.setvalue(float(li[6].text()), 150000, 1)
        y = self.ot.setvalue(float(li[7].text()), 150000, 1)
        z = self.ot.setvalue(float(li[8].text()), 60000, 1)

        b2 = bitstruct.pack("u18u19u19", *[z, y, x])
        metadata += b2[::-1]

        x = self.ot.setvalue(float(li[9].text()), 1500, 1)
        y = self.ot.setvalue(float(li[10].text()), 1500, 1)
        z = self.ot.setvalue(float(li[11].text()), 1500, 1)

        b2 = bitstruct.pack("u13u13u14", *[z, y, x])
        metadata += b2[::-1]

        x = self.ot.setvalue(float(li[12].text()), 150000, 1)
        y = self.ot.setvalue(float(li[13].text()), 150000, 1)
        z = self.ot.setvalue(float(li[14].text()), 60000, 1)

        b2 = bitstruct.pack("u18u19u19", *[z, y, x])
        metadata += b2[::-1]

        x = self.ot.setvalue(float(li[15].text()), 1500, 1)
        y = self.ot.setvalue(float(li[16].text()), 1500, 1)
        z = self.ot.setvalue(float(li[17].text()), 1500, 1)

        b2 = bitstruct.pack("u13u13u14", *[z, y, x])
        metadata += b2[::-1]

        b11 = struct.pack("H", *[0x1234])
        metadata += b11

        checksum = self.twdl_var.crc16(metadata)
        aa = struct.pack('>H', checksum)

        li1 = metadata[0:8] + aa + metadata[8:]
        li12 = bytearray(li1)

        self.fb.write(self.ot.GetDateTime())
        self.fb.write('\t')
        for i1 in range(len(li12)):
            self.fb.write("%02x " % (li12[i1]))
        self.fb.write('\n')
        self.fb.flush()

        self.twdl_var.pktB_log.write(li1)
        self.ff.udp_conn.sendto(li1, (self.ff.serverip, 6510))
        # self.ot.plot_data[1][0].append(self.counter2)
        # self.ot.pktA_seq[1].setData(self.ot.plot_data[1][0])
        # if self.counter2 % 10 == 0:
        #     self.ot.uplinkB_count.setText(
        #         "Packet B Sent: %d Received: %d MissCount: %d" % (self.counter2, self.ot.uplink_obc_count[1], self.ot.uplink_obc_miss[1]))


    def PacketStop(self, button, button1, index, index1):
        button1.setEnabled(True)
        button.setEnabled(True)
        if index == 0:
            self.packetA_timer.stop()
        elif index == 1:
            self.packetB_timer.stop()

        # self.ot.uplinkA_count.setText(
        #     "Packet A Sent: %d Received: %d MissCount: %d" % (self.counter1, self.ot.uplink_obc_count[0], self.ot.uplink_obc_miss[0]))
        # self.ot.uplinkB_count.setText(
        #     "Packet B Sent: %d Received: %d MissCount: %d" % (self.counter2, self.ot.uplink_obc_count[1], self.ot.uplink_obc_miss[1]))

    def PacketSend(self, button, index, index1):
        self.ot.uplink_obc_count[0] = 0
        self.ot.uplink_obc_count[1] = 0
        self.ot.uplink_obc_miss[0]=0
        self.ot.uplink_obc_miss[1] = 0
        self.counter1 = 0
        self.counter2 = 0

        if index == 0 and index1 == 0:
            self.sendPacketA()
        elif index == 0 and index1 == 1:
            button.setEnabled(False)
            self.packetB_timer.stop()
            self.packetA_timer.start()
        elif index == 1 and index1 == 0:
            self.sendPacketB()
        elif index == 1 and index1 == 1:
            button.setEnabled(False)
            self.packetA_timer.stop()
            self.packetB_timer.start()

        # self.ot.uplinkA_count.setText(
        #     "Packet A Sent: %d Received: %d MissCount: %d " % (self.counter1, self.ot.uplink_obc_count[0], self.ot.uplink_obc_miss[0]))
        # self.ot.uplinkB_count.setText(
        #     "Packet B Sent: %d Received: %d MissCount: %d" % (self.counter2, self.ot.uplink_obc_count[1], self.ot.uplink_obc_miss[1]))

    def CreateGraph(self, layout, widget, index, plot_attr):
        VBOX = QtGui.QVBoxLayout()
        self.ot.pyqtgraph[index] = pg.PlotWidget()
        self.ot.pyqtgraph[index].setFixedSize(widget.width() * 0.95, widget.height() * 0.25)
        self.pen = pg.mkPen(color=(255, 0, 0))
        self.pen1 = pg.mkPen(color=(0, 255, 0))
        self.ot.pyqtgraph[index].addLegend()
        self.ot.pyqtgraph[index].showGrid(x=True, y=True)
        self.ot.pyqtgraph[index].setTitle(" Plot ---")
        self.ot.pyqtgraph[index].setLabel('left', "<span style=\"color:red;font-size:20px\"> degrees</span>")
        self.ot.pyqtgraph[index].setLabel('bottom', '<span style="color:red;font-size:20px"> time in seconds </span>')
        self.ot.params[index] = self.ot.pyqtgraph[index].plot([], name=plot_attr, pen=self.pen)
        self.ot.params1[index] = self.ot.pyqtgraph[index].plot([], name=plot_attr+"-Loopback", pen=self.pen1)
        self.ot.plot_ready = True
        VBOX.addWidget(self.ot.pyqtgraph[index])

        vbox = QtGui.QVBoxLayout()
        vbox.setAlignment(QtCore.Qt.AlignRight | QtCore.Qt.AlignBottom)
        button = self.comp.getButton("Clear", 70, 25)
        QtCore.QObject.connect(button, QtCore.SIGNAL("clicked()"), lambda a=index: self.clear_graph(a))
        #vbox.addWidget(button)
        VBOX.addWidget(button)

        layout.addLayout(VBOX)

    def clear_graph(self, index):
        self.ot.pktA_seq[index] = self.ot.pyqtgraph[index].plot([], name="CCU-GDLC", pen=self.pen)
        self.ot.pktB_seq[index] = self.ot.pyqtgraph[index].plot([], name="OBC-MLP", pen=self.pen1)
        self.ot.plot_data[index][0] = []
        self.ot.plot_data[index][0] = []

    def TWDL_UI(self, widget, media_type):

        hbox = QtGui.QHBoxLayout(widget)

        vbox1 = QtGui.QVBoxLayout()
        vbox2 = QtGui.QVBoxLayout()

        control_frame = self.comp.createframe(self.missile_twdl_gbox.width() * 0.45,
                                              self.missile_twdl_gbox.height() * 0.04, -1, -1)
        control_frame_hl = QtGui.QHBoxLayout(control_frame)
        control_frame_hl.setContentsMargins(-10, 0, 0, 0)

        button1 = self.comp.createpushbutton(control_frame.width() * 0.3, control_frame.height() * 0.75, "Start")
        QtCore.QObject.connect(button1, QtCore.SIGNAL('clicked()'), lambda a=6, b=media_type: self.control_cmd(a, b))
        button2 = self.comp.createpushbutton(control_frame.width() * 0.3, control_frame.height() * 0.75, "Stop")
        QtCore.QObject.connect(button2, QtCore.SIGNAL('clicked()'), lambda a=7, b=media_type: self.control_cmd(a, b))

        # adding buttons to the frame
        control_frame_hl.addWidget(button1)
        control_frame_hl.addWidget(button2)

        #vbox2.addWidget(control_frame)

        length = len(self.twdl_var.twdl_gbox_name)
        for i in range(length):
            gb = self.comp.creategroupbox(widget.width() * 0.45, widget.height() * 0.35,
                                          self.twdl_var.twdl_gbox_name[i])

            if i in [1, 2]:  # height for config cmd and comm test response
                gb.setFixedHeight(widget.height() * 0.16)
                vbox1.addWidget(gb)
            elif i in [3]:  # version box
                gb.setFixedHeight(widget.height() * 0.20)
                vbox2.addWidget(gb)
            elif i in [4]:  # status and Configuration box
                gb.setFixedHeight(widget.height() * 0.7)
                vbox2.addWidget(gb)
            else:
                vbox1.addWidget(gb)
                gb.setFixedHeight(widget.height() * 0.5)

            gb_vbox = QtGui.QVBoxLayout(gb)

            HB = QtGui.QHBoxLayout()

            vb1 = QtGui.QVBoxLayout()
            vb2 = QtGui.QVBoxLayout()

            len1 = len(self.twdl_var.content_list[i])

            li = []
            ti = []
            for j in range(len1):

                if (i == 4) and (media_type == "Ethernet"):
                    label = self.comp.getLabel(self.twdl_var.config_ground_rsp_field[j], gb.width() * 0.4,
                                               gb.height() * 0.07)
                else:
                    label = self.comp.getLabel(self.twdl_var.content_list[i][j], gb.width() * 0.4, gb.height() * 0.07)

                # input list
                if i in [0, 1, 2]:
                    line = self.comp.getComboBox(self.twdl_var.combo_item_list[i][j], gb.width() * 0.4,
                                                 gb.height() * 0.08)

                    line.setCurrentIndex(self.twdl_var.combo_item_list_default[i][j])


                # output list
                elif i in [3]:
                    label.setFixedHeight(gb.height() * 0.1)
                    line = self.comp.getLineEdit(False, gb.width() * 0.4, gb.height() * 0.15)

                else:
                    line = self.comp.getLineEdit(False, gb.width() * 0.4, gb.height() * 0.05)

                # leaving 4 items to handle separately
                if i == 4:
                    if j < len1 - 4:
                        li.append(line)
                    else:
                        ti.append(line)
                else:
                    li.append(line)

                vb1.addWidget(label)
                vb2.addWidget(line)

                if i in [1, 2]:
                    label.setFixedHeight(gb.height() * 0.2)
                    line.setFixedHeight(gb.height() * 0.2)
                    line.setEnabled(True)

                if i == 0:
                    line.setEnabled(True)

            if media_type == 'MIL1553':
                if i == 4:
                    self.ot.twdl_map[int(self.twdl_var.message_id[i])] = li
                    self.ot.twdl_map[int(self.twdl_var.message_id[i + 1])] = ti
                else:
                    self.ot.twdl_map[int(self.twdl_var.message_id[i])] = li
            else:
                if i == 4:
                    self.ot.ground_twdl_map[int(self.twdl_var.ground_msg_id[i])] = li
                    self.ot.ground_twdl_map[int(self.twdl_var.ground_msg_id[i + 1])] = ti
                else:
                    self.ot.ground_twdl_map[int(self.twdl_var.ground_msg_id[i])] = li

            HB.addLayout(vb1)
            HB.addLayout(vb2)

            gb_vbox.addLayout(HB)

            if i in [0, 1, 2, 3]:
                hhbox = QtGui.QHBoxLayout()
                hhbox.setAlignment(QtCore.Qt.AlignBottom | QtCore.Qt.AlignCenter)
                hhbox.setContentsMargins(0, 10, 0, -5)
                button = self.comp.createpushbutton(60, 25, "Send")
                QtCore.QObject.connect(button, QtCore.SIGNAL('clicked()'), lambda a=i, b=media_type: self.cmd(a, b))
                hhbox.addWidget(button)
                gb_vbox.addLayout(hhbox)

                if i == 0:
                    button.setFixedHeight(gb.height() * 0.1)
                if i == 3:
                    button1 = self.comp.createpushbutton(60, 25, "Clear")
                    QtCore.QObject.connect(button1, QtCore.SIGNAL('clicked()'), lambda a=0: self.clear(a))
                    hhbox.addWidget(button1)


        hbox.addLayout(vbox1)
        hbox.addLayout(vbox2)

    def clear(self, index):
        #print self.ot.twdl_map[0xc]

        update_list = self.ot.twdl_map[0xc]
        update_list[0].setText("")
        update_list[1].setText("")
        update_list[2].setText("")

        update_list = self.ot.ground_twdl_map[0xe]
        update_list[0].setText("")
        update_list[1].setText("")
        update_list[2].setText("")


    def control_cmd(self, index, media_type):
        print index, media_type

        if media_type == "MIL1553":
            li = []
            li.append(1)
            li.append(1)
            li.append(int(index))
            li.append(32)
            li.append([])
            li.append(-1)
            self.flatc.PrepareObject(media_type, li)
        else:
            pass

        if index == 7:  # Twdl Stop
            self.ot.pktAtotal = 0
            self.ot.pktAPreSeqNum = 0
            self.ot.pktAmiss = 0

            self.ot.pktBtotal = 0
            self.ot.pktBPreSeqNum = 0
            self.ot.pktBmiss = 0

            self.ot.pktCtotal = 0
            self.ot.pktCPreSeqNum = 0
            self.ot.pktCmiss = 0

    def cmd(self, index, media_type):
        print index, ' media ', media_type

        if media_type == 'MIL1553':

            timetag = 0x00010001
            missile_id = int(self.ot.twdl_map[7][0].currentIndex()) + 1
            msg_id = self.twdl_var.message_id[index]

            obj_list = self.ot.twdl_map[msg_id]

            if self.ot.seq_count == 65535:
                self.ot.seq_count = 0
            else:
                self.ot.seq_count += 1
            # testing purpose
            # self.ot.seq_count = 1

            msg_seq_no = self.ot.seq_count

            data_payload = []
            data_payload.append(timetag)
            data_payload.append(int(msg_id << 8 | missile_id))
            data_payload.append(msg_seq_no)
            if index == 0:
                msb = self.twdl_var.cmd_items_value[index][1][obj_list[1].currentIndex()]
                lsb = self.twdl_var.cmd_items_value[index][2][obj_list[2].currentIndex()]
                data_payload.append(lsb << 8 | msb)

            if index in [0, 1, 2]:
                len1 = len(obj_list)
                print 'length of ', len1
                for i in range(len1):
                    # missile_id already taken
                    if index == 0 and i in [0, 1, 2]:
                        continue
                    else:
                        data_payload.append(self.twdl_var.cmd_items_value[index][i][obj_list[i].currentIndex()])

            checksum = self.checksum(data_payload, self.twdl_var.content_fmt_list[index])

            if self.twdl_var.obc_flag == 1:

                data_payload.insert(0, 0xaaaa)
                data_payload.insert(1, int(self.twdl_var.second_word[index]))
                data_payload.append(checksum)

                a = struct.pack('=2H' + self.twdl_var.content_fmt_list[index][1:], *data_payload)

                # version command  have both receive and transmit commands
                if index == 3:
                    self.flatc_payload(bytearray(a), index, media_type, 0)
                    time.sleep(.1)
                    self.flatc_payload(bytearray(a), index, media_type, 1)
                else:
                    self.flatc_payload(bytearray(a), index, media_type, 0)

            else:
                data_payload.append(checksum)

                a = struct.pack(self.twdl_var.content_fmt_list[index], *data_payload)

                self.flatc_payload(bytearray(a), index, media_type, 0)

            print 'FINAL list ', data_payload



        else:

            timetag = 0x00010001
            # print 'items', self.ot.twdl_map.keys() , '   ', self.ot.twdl_map[7]
            missile_id = int(self.ot.ground_twdl_map[4][0].currentIndex()) + 1
            msg_id = self.twdl_var.ground_msg_id[index]

            obj_list = self.ot.ground_twdl_map[msg_id]
            print 'obj_list', len(obj_list)

            if self.ot.seq_count == 65535:
                self.ot.seq_count = 0
            else:
                self.ot.seq_count += 1

            msg_seq_no = self.ot.seq_count

            data_payload = []
            data_payload.append(socket.htonl(timetag))
            data_payload.append(0xff)
            data_payload.append(int(msg_id))
            data_payload.append(socket.htons(msg_seq_no))

            if index == 0:
                # uplink frequency
                msb = self.twdl_var.cmd_items_value[index][1][obj_list[1].currentIndex()]
                data_payload.append(msb)
                data_payload.append(0)

                # downlink frequency
                lsb = self.twdl_var.cmd_items_value[index][2][obj_list[2].currentIndex()]
                data_payload.append(lsb)

                for i in range(9):
                    data_payload.append(0)

                # downlink cdma code
                data_payload.append((self.twdl_var.cmd_items_value[index][4][obj_list[4].currentIndex()]))

                # uplink cdma code
                data_payload.append((self.twdl_var.cmd_items_value[index][3][obj_list[3].currentIndex()]))
                for i in range(9):
                    data_payload.append(0)

            # only input list for 3 messages
            if index in [0, 1, 2]:
                len1 = len(obj_list)
                print 'length of ', len1
                for i in range(len1):
                    # missile_id already taken
                    if index == 0 and i in [0, 1, 2, 3, 4]:
                        continue;
                    else:
                        data_payload.append(self.twdl_var.cmd_items_value[index][i][obj_list[i].currentIndex()])

            checksum = 0
            checksum = self.checksum1(data_payload, self.twdl_var.ground_content_fmt_list[index])

            data_payload.insert(4, socket.htons(socket.htons(checksum)))
            print 'data_payload ---', data_payload, 'len---', len(data_payload)

            a = struct.pack(self.twdl_var.ground_content_fmt_list[index], *data_payload)

            self.ff.udp_conn.sendto(a, (self.ff.serverip,  6510))
            # self.flatc_payload(data_packet, index, media_type)

    def flatc_payload(self, data, index, media_type, dir):
        if media_type == "MIL1553":
            # print 'payload *******************' , data
            li = []
            li.append(self.twdl_var.rt_address)

            if index == 3 and dir == 0:
                li.append(4)
            else:
                li.append(self.twdl_var.subaddr_list[index])

            li.append(dir)
            if dir:
                li.append(32)
            else:
                li.append(len(data) / 2)
            li.append(data)
            li.append(1)

        else:
            li = []
            li.append(len(data))
            li.append(data)

        self.flatc.PrepareObject(media_type, li)

    def checksum(self, list1, fmt):
        print 'len ', list1, '  ', len(list1)

        fmt1 = '>' + fmt[1:-1]
        # print 'new string ', fmt1

        a = struct.pack(fmt1, *list1)

        f1 = open('temp-py.bin', 'wb')
        f1.write(a)
        f1.close()


        checksum = self.twdl_var.crc16(str(a))

        print 'checksum---', checksum
        return checksum

    def checksum1(self, list1, fmt):
        # print 'len ', list1, '  ', len(list1)

        fmt1 = fmt[0:4] + fmt[5:]
        print 'new string ', fmt1, '     ', list1
        a = struct.pack(fmt1, *list1)

        # f1 = open('temp-ground.bin', 'wb')
        # f1.write(a)
        # f1.close()

        checksum = self.twdl_var.crc16(str(a))

        print 'checksum---', hex(checksum)
        return checksum

    def Ground_TWDL_UI(self, widget):
        hbox = QtGui.QHBoxLayout(self.ground_twdl_gbox)
