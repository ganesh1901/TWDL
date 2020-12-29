import struct
import re
from PyQt4 import QtGui, QtCore
import os
import select
import crcmod
import socket
import math
import datetime
import time


class Others:
    def __init__(self):
        self.app = ''
        self.widget = ''
        self.app_name = 'TWDL'
        self.dated = "12-12-2020"
        self.MajorNo = 1
        self.MinorNo = 2
        self.Release = 3
        self.checksum = self.getchecksum()
        self.pyqtgraph = [[], [], [], [], [], [], [], []]
        self.seeker_plot = [[], []]
        self.ezimuth = [[], []]
        self.elevation = [[], []]
        self.timer = []
        self.timer1 = []
        self.samples_seeker = 100
        self.HStatus = ''
        self.statusbar = ''
        self.status_timer = QtCore.QTimer()
        self.status_timer.timeout.connect(self.status_timeout)

        self.username = 'Akashng'
        self.password = '12345'

        self.logfd = open("../out/app_log" + self.gettimedate(), "w+")

        self.stackWidget = ''

        self.pipe_data = []
        self.sur_pipe_data = []
        self.err_pipe_data = []

        self.ground_status = []
        self.ground_health = []

        self.pktAtotal = 0
        self.pktAPreSeqNum = 0
        self.pktAmiss = 0

        self.packet_A_timer = ''
        self.packet_B_timer = ''

        self.pktBtotal = 0
        self.pktBPreSeqNum = 0
        self.pktBmiss = 0

        self.pktCtotal = 0
        self.pktCPreSeqNum = 0
        self.pktCmiss = 0

        self.packetCounterList = []
        self.critical_val_offset = [21, 22, 28]
        self.st = StyleSheet()

        self.gain_validity = []

        self.update_others = {}

        self.dump_offset = [2, 3, 10, 11, 12, 13, 14, 15, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
                            28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47]

        self.realtime_map = {}
        self.realtime_map[0x0000] = "OBC REAL TIME"
        self.realtime_map[0x0001] = "HILS Mode"
        self.realtime_map[0x0101] = "SEAKER Mode"

        self.threashold_value = 0.5
        self.uplink = False
        self.downlink_data = []
        self.ground_map = {}
        self.twdl_raw_data1 = []
        self.onboard_map = {}
        self.plot_data = [[[], [], []], [[], [], []], [[], [], []]]
        self.x_plot_data = [[],[]]
        self.params = [[], [], [], []]
        self.params1 = [[], [], [], []]

        self.ema_command = [[], [], [], []]
        self.ema_incFb = [[], [], [], []]
        self.ema_absFb = [[], [], [], []]
        self.ema_current = [[], [], [], []]
        self.obc_demand = [[], [], [], []]
        self.ema_obc_time = []

        self.ema_command_data = [[], [], [], []]
        self.ema_incFb_data = [[], [], [], []]
        self.ema_absFb_data = [[], [], [], []]
        self.ema_current_data = [[], [], [], []]
        self.obc_demand_data = [[], [], [], []]

        self.ins_time = 0
        self.seeker_adata = []
        self.seeker_bdata = []

        self.FrameBobjectMap = {}
        # This map takes index as key and a list(lineEditObject, BitInfo if exists otherwise -1) as value
        self.FrameAobjectMap = {}

        self.uplinkA_counts = ""
        self.uplinkB_counts = ""
        self.downlink_counts = ""

        self.ground_dnlink_count = 0
        self.ground_dnlink_pseq = 0
        self.ground_dnlink_cseq = 0
        self.ground_dnlink_miss = 0

        self.uplink_obc_count = [0, 0]
        self.uplink_obc_pseq = [0, 0]
        self.uplink_obc_cseq = [0, 0]
        self.uplink_obc_miss = [0, 0]
        self.twdl_map = {}
        self.ground_twdl_map = {}
        self.adc_pipe_data = []
        self.global_twdl = ''
        self.adc_flag =  False

        self.Tgt_init_pos_LCI = [12000.0, 0.0, -4000.0]
        self.Tgt_init_vel_LCI = [-150.0, 0.0, 0.0]
        self.crc16 = crcmod.mkCrcFun(0x11021, 0x1d0f, 0, 0)
        self.ema_pipe_data = []
        self.levelling_flag = True

    def getScreenShot(self, name):
        self.screen = QtGui.QApplication.primaryScreen()
        screenshot = self.screen.grabWindow(self.app.desktop().winId())
        screenshot.save('../out/' + name + self.gettimedate() + '.png')


    def setvalue(self, val, offset, scale):
        temp = int((val + offset) / scale)
        # print "val- %d offset- %d scale- %d result- %f" % (val, offset, scale, temp)
        return temp

    def getchecksum(self):
        cwd = os.getenv('PWD')
        os.system('../run_scripts/checksum.sh')
        f = open('/tmp/cfg', 'r')
        buf = ''
        if f:
            buf = f.readline()
            print 'Data', buf
            f.close()
            self.checksum = buf.split(' ')[0][-8:]
        return self.checksum

    def getappsize(self):
        return str(os.stat('../bin/simulator').st_size) + str('  in bytes')

    def getControllersize(self):
        return str(os.stat('../bin/simulator').st_size) + str('  in bytes')

    #return date time
    def gettimedate(self):
        curr_time_object = datetime.datetime.now()
        curr_time = "-%02d-%02d-%04d-%02d-%02d-%02d" % (
            curr_time_object.day, curr_time_object.month, curr_time_object.year, curr_time_object.hour,
            curr_time_object.minute, curr_time_object.second)
        return curr_time

    #return date time millisecond
    def GetDateTime(self):
        curr_time_object = datetime.datetime.now()
        curr_time = "%02d%02d%02d %02d:%02d:%02d:%03d" % (
            curr_time_object.day, curr_time_object.month, curr_time_object.year, curr_time_object.hour,
            curr_time_object.minute, curr_time_object.second, curr_time_object.microsecond/1000)
        return curr_time

    def logDebug(self, text):
        self.logfd.write(self.gettimedate() + '\t')
        self.logfd.write("Debug:\t")
        self.logfd.write(text)
        self.logfd.write("\n")
        self.logfd.flush()

    def logWarning(self, text):
        self.logfd.write(self.gettimedate() + '\t')
        self.logfd.write("Warning:\t")
        self.logfd.write(text)
        self.logfd.write("\n")
        self.logfd.flush()

    def status_timeout(self):
        self.status_timer.stop()
        self.statusbar.setText('')

    def notifyStatus(self, notify_text, notify_type):

        if notify_type == 1:
            self.statusbar.setText(notify_text)
            self.statusbar.setStyleSheet(self.st.valid_label)
            self.logDebug(notify_text)

        elif notify_type == 2:
            self.statusbar.setText(notify_text)
            self.statusbar.setStyleSheet(self.st.notvalid_label)
            self.logWarning(notify_text)
        else:
            self.statusbar.setText(notify_text)
            self.statusbar.setStyleSheet(self.st.default_label)
            self.logDebug(notify_text)

        self.status_timer.start(1000)

        self.realtime_map = {}
        self.realtime_map[0x0000] = "OBC REAL TIME"
        self.realtime_map[0x0001] = "HILS Mode"
        self.realtime_map[0x0101] = "SEAKER Mode"

class TWDL:
    def __init__(self):

        # flag for OBC or without OBC
        self.obc_flag = 1
        self.msg_seq_number = 0
        self.twdl_gbox_name = ["Configuration Command", "PA Config", "TX Antenna", "Version Details", "Status--Health"]
        self.second_word = [0x5443, 0x5450, 0x5458, 0x5456, 0x5448, 0x5454]

        self.message_id = [0x7, 0xA, 0xB, 0xC, 0x9, 0x8, 0xD]
        self.config_cmd_field = ["Missile Id", "Uplink Freq", "Dnlink Freq", "Uplink CDMA", "Dnlink CDMA",
                                 "Dn & Up FEC", "PA Mode", "TX Antn"]

        self.config_cmd_fmt = "=I8HH"
        self.config_cmd_items = [[1, 2, 3, 4, 5, 6, 7, 8, 9, 10], [0, 1, 2, 3, 4, 5, 6, 7, 8],
                                 [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC], [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA],
                                 [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA], ["ON &OFF", "ON & ON", "OFF & ON", "OFF & OFF"],
                                 ["OFF", "1 watt", "5 watt", "10 watt", "15 watt"], ["Port1", "Port2"]]

        self.config_cmd_items_value = [[1, 2, 3, 4, 5, 6, 7, 8, 9, 10], [0, 1, 2, 3, 4, 5, 6, 7, 8],
                                       [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA, 0xB, 0xC],
                                       [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA],
                                       [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0xA], [0x00ff, 0xffff, 0xff00, 0x0000],
                                       [0x0000, 0xFF01, 0xff02, 0xff03, 0xff04], [0, 0xffff]]

        self.pa_cmd_field = ["Power MODE"]
        self.pa_cmd_items = [["OFF", "1 watt", "5 watt", "10 watt", "15 watt"]]
        self.pa_cmd_items_value = [[0x0000, 0xFF01, 0xff02, 0xff03, 0xff04]]
        self.pa_cmd_fmt = "=I3HH"

        self.tx_switchover_cmd_field = ["Tx Antn"]
        self.tx_switchover_cmd_items = [["Port1", "Port2"]]
        self.tx_switchover_cmd_items_value = [[0, 0xffff]]
        self.tx_switchover_cmd_fmt = "=I3HH"

        self.version_details_field = ["MissileId", "Version", "Dated"]
        self.version_cmd_fmt = '=IHHH'

        self.combo_item_list = []
        self.combo_item_list.append(self.config_cmd_items)
        self.combo_item_list.append(self.pa_cmd_items)
        self.combo_item_list.append(self.tx_switchover_cmd_items)

        self.combo_item_list_default = []
        self.combo_item_list_default.append([0, 0, 0, 1, 1, 3, 0, 1])
        self.combo_item_list_default.append([0])
        self.combo_item_list_default.append([1])

        # self.config_rsp_field = ["Missile Id", "Uplink Freq", "Dnlink Freq","Uplink CDMA", "Dnlink CDMA", "PA Status", "RF status", "TX Power Mode", "RSSI-RX1", "RSSI-RX2", "Synthesizer Lock", "Uplink FEC", "Dnlink FEC"]
        self.config_onboard_rsp_field = ["Missile Id", "UL-Freq", "DL-Freq", "UL-CDMA", "DL-CDMA", "TX Ant",
                                         "PA Status", "RSSI-RX1", "RSSI-RX2", "RX1 Lock", "RX2 Lock", "Tx Lock",
                                         "RX1-Dec",
                                         "RX2-Dec", "Dnlink FEC", "Uplink FEC", "RX1-Dopp",
                                         "RX2 Dopp", "RX1-Corr", "RX2-Corr"]

        '''
        self.config_ground_rsp_field = ["Missile Id", "UL-Freq", "DL-Freq", "UL-CDMA", "DL-CDMA", "TX Ant",
                                 "PA Status", "RSSI-RX1", "RSSI-RX2", "Tx1 Lock", "Tx2 Lock", "Rx Lock", "RX1-Dec",
                                 "RX2-Dec", "Uplink FEC", "Dnlink FEC", "RX1-Dopp",
                                 "RX2 Dopp", "RX1-Corr", "RX2-Corr"]
        '''

        if self.obc_flag == 1:
            self.config_rsp_fmt = "=HHI2BH2BHHHH8BH16H"
            self.version_rsp_fmt = '=HHI2BH2B2H23H'
            self.onboard_config_rsp_ui_offset = [0, 1, 2, 3, 4, 7, 8, 9, 10, 11]
            self.config_rsp_field_index = [3, 6, 7, 8, 9, 12, 13, '14|1', '14|2', '15|1']

        else:
            self.config_rsp_fmt = "=I2BH2BHHHH8BH18H"
            self.version_rsp_fmt = '=I2BH3H23H'
            self.config_rsp_field_index = [1, 4, 5, 6, 7, 8, 9, 10, 11, '12|1', '12|2', '13|1', 14, 15, 16, 17]

        self.pastatus_map = {}

        self.config_rsp_alias_map = {}

        if self.obc_flag == 1:
            self.health_rsp_fmt = "=HHIBBH2BBBBB6H17H"
            # self.health_rsp_field_index = ['6|1', '6|2', '7|1', 8, 9, 13, 14, 15, 16] #merged the fields to status and health
            self.health_rsp_field_index = [13, 14, 15, 16]
        else:
            self.health_rsp_fmt = "=IBBH2BBBBB6H19H"
            self.health_rsp_field_index = ['4|1', '4|2', '5|1', 6, 7, 10, 11, 12, 13]

        self.comm_test_resp1 = ["Tx Cmd Flag", "Target Id", "Target Type"]
        self.comm_test_resp2 = ["Pos", "Vel"]
        self.comm_test_resp_fmt = "=HHI2BHH2B3f3hH14H"
        self.comm_test_resp_index = [6, 7, 8, 9, 10, 11, 12, 13, 14]

        self.content_list = []
        self.content_list.append(self.config_cmd_field)
        self.content_list.append(self.pa_cmd_field)
        self.content_list.append(self.tx_switchover_cmd_field)
        self.content_list.append(self.version_details_field)
        self.content_list.append(self.config_onboard_rsp_field)
        self.content_list.append(self.comm_test_resp1)

        self.content_fmt_list = []
        self.content_fmt_list.append(self.config_cmd_fmt)
        self.content_fmt_list.append(self.pa_cmd_fmt)
        self.content_fmt_list.append(self.tx_switchover_cmd_fmt)
        self.content_fmt_list.append(self.version_cmd_fmt)

        self.cmd_items_value = []
        self.cmd_items_value.append(self.config_cmd_items_value)
        self.cmd_items_value.append(self.pa_cmd_items_value)
        self.cmd_items_value.append(self.tx_switchover_cmd_items_value)

        if self.obc_flag == 1:
            self.subaddr_list = [4, 4, 4, 6]
            self.rt_address = 1
            self.health_data_subadd = 12
            self.status_data_subadd = 13
            self.data_subaddr = [14, 4, 15]
            self.version_subaddr = 6

        else:
            self.subaddr_list = [1, 4, 5, 10]
            self.rt_address = 2
            self.health_data_subadd = 7
            self.status_data_subadd = 8
            self.data_subaddr = [1, 2, 3]
            self.version_subaddr = 6


        self.health_data = []
        self.status_data = []

        self.crc16 = crcmod.mkCrcFun(0x11021, 0x1d0f, 0, 0)
        self.ground_msg_id = [4, 0xc, 0xd, 0xe, 8, 7]

        self.ground_content_fmt_list = []
        self.ground_content_fmt_list.append('>I2BHH12B11H3H')
        self.ground_content_fmt_list.append('>I2BHHHHH')
        self.ground_content_fmt_list.append('>I2BHHHH')
        self.ground_content_fmt_list.append('>I2BHH')


        self.ground_version_rsp_fmt = ">IBBHHIBBBBHI"

        self.config_ground_rsp_field = ["Missile Id", "UL-Freq", "DL-Freq", "UL-CDMA", "DL-CDMA", "TX Ant",
                                        "PA Status", "RSSI-RX1", "RSSI-RX2", "Tx1 Lock", "Tx2 Lock", "Rx Lock",
                                        "RX1-Dec", "RX2-Dec", "Dnlink FEC", "Uplink FEC", "RX1-Dopp",
                                        "RX2 Dopp", "RX1-Corr", "RX2-Corr"]

        self.ground_config_rsp_bytes_status_offset = [6, 8, 18, 19, 31, 32]
        self.ground_config_rsp_ui_status_offset = [1, 2, 3, 4, 7, 8]

        self.ground_config_rsp_ui_health_offset = [0, 1, 2, 3]
        self.ground_config_rsp_bytes_health_offset = [13, 14, 15, 16]


        self.data_items = ["Packet-A   CCU-GDLC   OBC_MLP", "Packet-B   CCU_GDLC   OBC-MLP", "Packet-C  OBC-MLP   GDL-CCU"]

        self.pktA_items = ["CCU-Msg Seq No", "TIMETAG", "MissileID", "MLV ID", "FU-ID", "Seq No",
                           "Target Position Xt", "Target Position Yt", "Target Position Zt",
                           "Target Velocity Xt", "Target Velocity Yt", "Target Velocity Zt",
                           "Target Accel Axt", "Target Accel Ayt", "Target Accel Azt",
                           "Variance Pos Xt", "Variance Pos Yt", "Variance Pos Zt",
                           "Variance Vel Xt", "Variance Vel Yt", "Variance Vel Zt",
                           "Target Type", "Target RCS"]

        self.pktA_items_default = [1, 12345678, 1, 3, 0, 1, 12000, 0, 4000, 111, -111, 111, 122, 122, 122,
                                   100, 100, 100, 55, 55, 55, 2, 4]

        #self.pktA_items_default_setlimits = [[0, 65535], [0, 65535], [0, 10], [0, 63], [0, 3], [0, 65535], ]

        self.pktB_items = ["CCU-Msg Seq No", "TIMETAG", "MissileID", "MLV ID", "FU-ID", "Seq No",
                           "Target Position Xt", "Target Position Yt", "Target Position Zt",
                           "Target Velocity Vx", "Target Velocity Vy", "Target Velocity Vz",
                           "Missile Position Xt", "Missile Position Yt", "Missile Position Zt",
                           "Missile Velocity Vx", "Missile Velocity Vy", "Missile Velocity Vz"]

        self.pktB_items_default = [1, 12345678, 1, 3, 0, 1, 12000, 0, 4000, -222, 222, 222, 111, -111, 111, 44, -44, 44]

        self.pkt_items_default = []
        self.pkt_items_default.append(self.pktA_items_default)
        self.pkt_items_default.append(self.pktB_items_default)


        self.pktC_items = ["TIMETAG", "MissileID", "MLV ID", "FU-ID", "Seq No", "MsgId",
                           "Missile Position Xt", "Missile Position Yt", "Missile Position Zt",
                           "Missile Velocity Vx", "Missile Velocity Vy", "Missile Velocity Vz",
                           "Missile Accel Axt", "Missile Accel Ayt", "Missile Accel Azt",
                           "body rates P", "body rates Q", "body rates R",
                           "Euler Rates SI", "Euler Rates PSI", "Euler Rates Theta",
                           "deg1", "deg2", "deg3", "deg4", "Msl Status", "Seeker Status"]

        self.data_items_list = []
        self.data_items_list.append(self.pktA_items)
        self.data_items_list.append(self.pktB_items)
        self.data_items_list.append(self.pktC_items)

        self.pktA_log = open("../out/UplinkA.bin", "wb")
        self.pktB_log = open("../out/UplinkB.bin", "wb")


class FIFO:
    def __init__(self):
        try:

            self.rx_fifo = "/tmp/cmd_fifo"  # named fifo for sending command
            self.tx_fifo = "/tmp/rsp_fifo"  # named fifo for receiving response
            self.err_fifo = "/tmp/err_fifo"
            self.surv_fifo = "/tmp/sur_fifo"
            self.adc_fifo = "/tmp/adc_fifo"
            self.ema_fifo = "/tmp/ema_fifo"

            self.max_fifo_size = 512

            if not os.access(self.rx_fifo, 6):
                os.mkfifo(self.rx_fifo, 0600)
            self.cmd_fd = os.open(self.rx_fifo, os.O_RDWR | os.O_NONBLOCK)
            print 'cmd fifo', self.cmd_fd

            if not os.access(self.tx_fifo, 6):
                os.mkfifo(self.tx_fifo, 0600)
            self.rsp_fd = os.open(self.tx_fifo, os.O_RDWR | os.O_NONBLOCK)
            print 'rsp fifo ', self.rsp_fd

            if not os.access(self.surv_fifo, 6):
                os.mkfifo(self.surv_fifo, 0600)
            self.sur_fd = os.open(self.surv_fifo, os.O_NONBLOCK | os.O_RDWR)
            print 'sur fifo', self.sur_fd

            if not os.access(self.err_fifo, 6):
                os.mkfifo(self.err_fifo, 0600)
            self.err_fd = os.open(self.err_fifo, os.O_NONBLOCK | os.O_RDWR)
            print 'err fifo', self.err_fd

            if not os.access(self.adc_fifo, 6):
                os.mkfifo(self.adc_fifo, 0600)
            self.adc_fd = os.open(self.adc_fifo, os.O_NONBLOCK | os.O_RDWR)
            print 'adc fifo', self.adc_fd

            if not os.access(self.ema_fifo, 6):
                os.mkfifo(self.ema_fifo, 0600)
            self.ema_fd = os.open(self.ema_fifo, os.O_NONBLOCK | os.O_RDWR)
            print 'ema fifo', self.ema_fd

            self.sock_list = []
            self.sock_list.append(self.sur_fd)
            self.sock_list.append(self.rsp_fd)
            self.sock_list.append(self.err_fd)
            self.sock_list.append(self.adc_fd)
            self.sock_list.append(self.ema_fd)

            self.host_addr = "0.0.0.0"

            self.port_list = [6511, 6510]
            #self.port_list = [6521, 6520]
            self.serverip = "10.90.1.77"

            self.fd_map_signal = {}
            self.fd_map_signal[self.rsp_fd] = ''
            self.fd_map_signal[self.sur_fd] = ''
            self.fd_map_signal[self.ema_fd] = ''

            self.udp_conn = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            self.udp_conn.bind((self.host_addr, self.port_list[0]))
            self.udp_poll = select.epoll()
            self.udp_poll.register(self.udp_conn.fileno())
            self.poll_timeout = 1
            self.udp_conn.setblocking(0)

        except IOError as IOE:
            print IOE
        except OSError as OSE:
            print OSE

class StyleSheet:
    def __init__(self):
        self.frame = '''
            .QFrame{
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #192f33, stop: 1#192f33); 
                font-family:Georgia;
                font-size :18px;
                font-weight:bold;
                color:#000000;
                border: 2px solid rgb(25, 47, 51) ;
            }
            '''
        self.frame_noborder = '''
            .QFrame{
              background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dae3de, stop: 1#dae3de); 
              font-family:Georgia;
              font-size :14px;
              font-weight:bold;
              color:#000000;
              border: 1px dotted rgb(255, 255, 255) ;                      
            }
            '''
        self.header_label = '''
            .QLabel{
             font-family:Blatant;
             font-size :28px;
             font-weight:bold;
             color:#000000;
            }
            '''
        self.cmd_label = '''
            .QLabel{
             font-family:Times New Roman;
             font-size :16px;
             font-weight:bold;
             color:#a0522d;
            }
            '''
        self.rsp_label = '''
        .QLabel{
         font-family:Times New Roman;
         font-size :14px;
         font-weight:bold;
         color:rgb(200,145,12);
        }
        '''

        self.body_frame = '''
            .QFrame{
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #f3f2f2, stop: 1#f2f2f2); 
                font-family:Georgia;
                font-size :18px;
                font-weight:bold;
                color:#000000;
    
            }
        '''

        self.item_frame = '''
            .QLabel{
                font-family:Georgia;
                font-size :17px;
                font-weight:bold;
                color:#a0522d;
                text-align:justify; 
                border:none;          
            }
    
            .QLineEdit{
                background-color : rgb(255, 255, 255); 
                font-family:Georgia;
                font-size :18px;
                font-weight:bold;
                text-align: justify;
                right:0.1em;
                color:#000000;     
                border:1px solid green;
            }
    
            .QLineEdit:read-only {
                background: lightblue;
            }
        '''
        self.item_frame1 = '''
            .QLabel{
                font-family:Georgia;
                font-size :17px;
                font-weight:bold;
                color:#8000ff;
                text-align:justify; 
                border:none;          
            }

            .QLineEdit{
                background-color : rgb(255, 255, 255); 
                font-family:Georgia;
                font-size :18px;
                font-weight:bold;
                text-align: justify;
                right:0.1em;
                color:#000000;     
                border:1px solid green;
            }

            .QLineEdit:read-only {
                background: lightblue;
            }
        '''
        self.test_edit_style = '''
            .QTextEdit{
                background-color : rgb(255, 255, 255); 
                font-family:Georgia;
                font-size :16px;
                font-weight:bold;
                color:#000000;     
                border:2px dashed gray;
            }
        '''

        self.enable_pushbnt = '''
            .QPushButton{
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0      #BDB76B stop: 1 #148f77);   
                    font-family:Georgia;
                    font-size :14px;
                    font-weight:bold;
                    color:#ffffff;
                    border-radius: 15;
            }

            QPushButton:hover
            {
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0      #BDB76B stop: 1 #FFDAB9);
                color:#000000;
            }
        '''

        self.disable_pushbtn = '''
            .QPushButton{
                background-color:#000000;   
                font-family:Georgia;
                font-size :14px;
                font-weight:bold;
                color:#ffffff;
                border-radius: 15;
            }
        '''

        self.plot_cbox = '''
            .QCheckBox{
                font-family:Georgia;
                font-size :18px;
                font-weight:bold;
                color:black;
                text-align:justify; 
                border:none; 

            }
        '''
        self.plot_rbutton = ''' 
            .QRadioButton{
                font-family:Georgia;
                font-size :18px;
                font-weight:bold;
                color:black;
                text-align:justify; 
                border:none; 

            }
        '''

        self.default_label = '''
            .QLabel{
             font-family:Times New Roman;
             font-size :16px;
             font-weight:bold;
             color:rgb(169, 169, 169);
            }
            '''

        self.notvalid_label = '''
                .QLabel{
                 font-family:Times New Roman;
                 font-size :20px;
                 font-weight:bold;
                 color:rgb(255, 0, 0);
                 border:1px solid black;
                }
                '''

        self.valid_label = '''
                .QLabel{
                 font-family:Times New Roman;
                 font-size :20px;
                 font-weight:bold;
                 color:rgb(0, 0, 255);
                 border:1px solid black;
                }
                '''

        self.default_status = '''
                    .QStatusBar{
                     font-family:Times New Roman;
                     font-size :16px;
                     font-weight:bold;
                     color:rgb(169, 169, 169);
                     border:1px solid black;
                    }
                    '''

        self.notvalid_status = '''
                        .QStatusBar{
                         font-family:Times New Roman;
                         font-size :20px;
                         font-weight:bold;
                         color:rgb(255, 0, 0);
                        }
                        '''

        self.valid_status = '''
                        .QStatusBar{
                         font-family:Times New Roman;
                         font-size :20px;
                         font-weight:bold;
                         color:rgb(0, 0, 255);
                        }
                        '''

        self.status_label = '''
            .QLabel{
                     font-family:Times New Roman;
                     font-size :14px;
                     font-weight:bold;
                     color:rgb(0, 0, 255);
                    }
            '''
        self.gbox = """
            .QGroupBox {
                font: bold"Times New Roman";    
                font-size:19px;
                border: 3px solid gray;
                border-radius:2px;
                margin-top:0.7em;
            left:0.01em;
            top:-2em;
                background-color : rgb(255, 255, 255);
                
            }
            .QGroupBox::title{
                    subcontrol-origin:margin;
                    padding:0 1px 0 3px;
                    subcontrol-position: top center; /* position at the top center */
                  
            }
            
            .QLabel{
                font-family:Georgia;
                font-size :14px;
                font-weight:bold;
                color:#a0522d;
                text-align:justify; 
                border:none;          
            }
            .QLineEdit{
                background-color : rgb(255, 255, 255); 
                font-family:Georgia;
                font-size :16px;
                font-weight:bold;
                text-align: justify;
                right:0.1em;
                color:#000000;     
                border:2px solid green;
            }
            .QTextEdit{
            background-color : rgb(255, 255, 255); 
                font-family:Georgia;
                font-size :16px;
                font-weight:bold;
                color:#000000;     
                border:2px dashed gray;
            }
            
            .QLineEdit:read-only {
                background: lightblue;
            }
            
            .QCheckBox{
                font-family:Georgia;
                font-size :14px;
                font-weight:bold;
                color:#a0522d;
                text-align:justify; 
                border:none; 
            
            }
            
            .QRadioButton{
                font-family:Georgia;
                font-size :14px;
                font-weight:bold;
                color:#a0522d;
                text-align:justify; 
                border:none; 
            
            }
            .QPushButton{
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #666600 stop: 1 #9ACD32);    
                font-family:Georgia;
                font-size :15px;
                font-weight:bold;
                color:#ffffff;
                border-radius: 10;
            }
            
            QPushButton:hover
            {
                   background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0      #BDB76B stop: 1 #FFDAB9);
                   color:#000000;
            }
            
            """

        self.line_edit = """
            .QLineEdit{
                    background-color : rgb(255, 255, 255); 
                    font-family:Georgia;
                    font-size :16px;
                    font-weight:bold;
                    text-align: justify;
                    right:0.1em;
                    color:#000000;     
                    border:2px solid green;
                }
        """
        self.line_edit_readonly = """
            .QLineEdit{
                    background-color : rgb(255, 255, 255); 
                    font-family:Georgia;
                    font-size :16px;
                    font-weight:bold;
                    text-align: justify;
                    right:0.1em;
                    color:#000000;     
                    border:2px solid black;
                }
        """

        self.line1 = """
        .QLineEdit{
                    background-color : rgb(255, 255, 255); 
                    font-family:Georgia;
                    font-size :16px;
                    font-weight:bold;
                    text-align: justify;
                    right:0.1em;
                    color:#000000;     
                    border:2px solid green;
                }
	    """

        self.line_OK = """
        	.QLineEdit{
        		        background-color : rgb(255, 255, 255); 
        		        font-family:Georgia;
        		        font-size :16px;
        		        font-weight:bold;
        		        text-align: justify;
        		        right:0.1em;
        		        color:#0000ff;     
        		        border:2px solid black;
        		    }
        	"""

        self.line_highlight = """
                	.QLineEdit{
                		        background-color : rgb(255, 255, 255); 
                		        font-family:Georgia;
                		        font-size :32px;
                		        font-weight:bold;
                		        text-align: justify;
                		        right:0.1em;
                		        color:#0000ff;     
                		        border:2px solid black;
                		    }
                	"""

        self.line_notok = """
        	.QLineEdit{
        		        background-color : rgb(255, 255, 255); 
        		        font-family:Georgia;
        		        font-size :16px;
        		        font-weight:bold;
        		        text-align: justify;
        		        right:0.1em;
        		        color:#ff0000;     
        		        border:2px solid black;
        		    }
        	"""
        self.label = '''
            .QLabel{
                font-family:Times New Roman;
                font-size :18px;
                font-weight:bold;
                color:#006600;
            }
            '''

        self.hils_frame_label_start = '''
        
        QLabel{
            background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fdf5e6 stop: 1 #fdf5e6);	
            font-family:Georgia;
            font-size :18px;
            font-weight:bold;
            color:#000000;
            border-radius: 10;
        }
        
        '''

        self.hils_frame_label_stop = '''
        
        QLabel{
            background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #3cb371 stop: 1 #3cb371);	
            font-family:Georgia;
            font-size :18px;
            font-weight:bold;
            color:#ffffff;
            border-radius: 10;
        }
        
        
        '''

        self.table_header_style = "::section {""background-color: #808b96; color: #4b0082; font-size:20px; font-family:Times New Roman; font-weight:bold;}"

        self.table_header_style1 = "::section {""background-color: #808b96; color:#4b0082 ; font-size:15px; font-family:Times New Roman; font-weight:bold;}"

        self.table_style = '''
            QTableView{
            border: 2px solid;
            background:#fffafa;
            color:sienna;
            gridline-color: solid black;
            }
            '''

        self.tree_view_style_sheet = '''
            
            QTreeView{
                color:#800080;
            }
            
            
            QTreeView::item:hover{
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1 stop: 1 #00bfff);
                border: 1px solid #000;
                color: #ffffff;
            }
            
            QTreeView::item:selected{
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1 stop: 1 #567dbc);
            }
            
            QTreeView::item:selected !active{
                background-color:red;
            }
        '''

        self.treeview_font = QtGui.QFont("Georgia", 12, QtGui.QFont.Bold)

        self.push_button = '''
            .QPushButton{
                background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #666600 stop: 1 #9ACD32);    
                font-family:Georgia;
                font-size :18px;
                font-weight:bold;
                color:#ffffff;
                border-radius: 10;
            }
            
            QPushButton:hover
            {
                   background-color:qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0      #BDB76B stop: 1 #FFDAB9);
                   color:#000000;
            }
            '''
        self.table_header_style = "::section {""background-color: #808b96; color: #4b0082; font-size:20px; font-family:Times New Roman; font-weight:bold;}"

        self.table_header_style1 = "::section {""background-color: #808b96; color:#4b0082 ; font-size:15px; font-family:Times New Roman; font-weight:bold;}"

        self.table_style = '''
        QTableView{
        
            border: 2px solid;
            background:#fffafa;
            color:sienna;
            gridline-color: solid black;
        }
        
        '''
        self.textedit = """
        .QTextEdit
        {
            background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fffafa, stop: 1 #fffafa);
            font-family:Georgia;
            font-size:16px;
            font-weight:bold;
            color:#000000;
            border:1px solid;
            border-radius: 5;    
        }
        """

        self.statusbar = """
          .QStatusBar{
                     font-family:Times New Roman;
                     font-size :16px;
                     font-weight:bold;
                     border: 2px solid ;
                    }
        """

        self.tabWidget = '''
        
        .QTabWidget::pane { /* The tab widget frame */
            border: 1px dashed rgb(100, 100, 100);
        }
        .QTabWidget::tab-bar {
            top: -0.2em; /* move to the right by 5px */
        }
        /* Style the tab using the tab sub-control. Note that it reads QTabBar _not_ QTabWidget */
        .QTabBar::tab {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #E1E1E1, stop: 1.0 #D3D3D3);
            border: 1px solid #C4C4C3;
            border-bottom-color: #C2C7CB; /* same as the pane color */
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            min-width: 8ex;
            padding: 2px;
        }
        .QTabBar::tab:selected, QTabBar::tab:hover {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #fafafa, stop: 0.4 #f4f4f4, stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);
        }
        .QTabBar::tab:selected {
            border-color: #9B9B9B;
            border-bottom-color: #C2C7CB; /* same as pane color */
        }
        .QTabBar::tab:!selected {
            margin-top: 2px; /* make non-selected tabs look smaller */
        }   
        '''

        self.tabWidget1 = '''
                    .QTabWidget::pane{ /* The tab widget frame */
                        border:1px dashed  #C337CB;
                top: -0.2em;
                left: 0.2em;
                    }   
            '''

        self.stack_widget = '''
                .QStackedWidget{
                    border: 2px solid rgb(0, 0, 0) 
                }
            
            '''
