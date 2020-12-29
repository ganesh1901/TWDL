import select
import os
from PyQt4 import QtCore
from PyQt4.QtCore import QThread


class ReceiverHandler(QThread):
    rsp_signal = QtCore.pyqtSignal()
    sur_signal = QtCore.pyqtSignal()
    err_signal = QtCore.pyqtSignal()
    adc_signal = QtCore.pyqtSignal()
    ema_signal = QtCore.pyqtSignal()

    def __init__(self, ot, ff, flatc):
        QThread.__init__(self)
        self.ot = ot
        self.ff = ff
        self.flatc = flatc

        self.rsp_signal.connect(self.decodeData)
        self.sur_signal.connect(self.decodeSurData)
        self.err_signal.connect(self.decodeErrData)
        self.adc_signal.connect(self.adcData)
        self.ema_signal.connect(self.emaData)

        li = []
        li.append(self.ot.pipe_data)
        li.append(self.rsp_signal)
        self.ff.fd_map_signal[self.ff.rsp_fd] = li

        li = []
        li.append(self.ot.sur_pipe_data)
        li.append(self.sur_signal)
        self.ff.fd_map_signal[self.ff.sur_fd] = li

        li = []
        li.append(self.ot.err_pipe_data)
        li.append(self.err_signal)
        self.ff.fd_map_signal[self.ff.err_fd] = li

        li = []
        li.append(self.ot.adc_pipe_data)
        li.append(self.adc_signal)
        self.ff.fd_map_signal[self.ff.adc_fd] = li

        li = []
        li.append(self.ot.ema_pipe_data)
        li.append(self.ema_signal)

        self.ff.fd_map_signal[self.ff.ema_fd] = li

        self.start()

    def run(self):
        while True:
            try:
                read_list, write_list, error_list = select.select(self.ff.sock_list, [], [], 10)
                for fd in read_list:
                    data = os.read(fd, self.ff.max_fifo_size)
                    #print data
                    self.ff.fd_map_signal[fd][0].append(data)
                    self.ff.fd_map_signal[fd][1].emit()

            except IOError:
                print ' system error '
            except AttributeError as ae:
                print ' *******', ae

    @QtCore.pyqtSlot()
    def decodeData(self):
        self.flatc.DecodeObject(self.ot.pipe_data.pop())

    @QtCore.pyqtSlot()
    def decodeSurData(self):
        self.flatc.Survelliance_DecodeData(self.ot.sur_pipe_data.pop())

    @QtCore.pyqtSlot()
    def decodeErrData(self):
        self.ot.notifyStatus(self.ot.err_pipe_data.pop(), 2)

    @QtCore.pyqtSlot()
    def adcData(self):
        self.flatc.DecodeADCData(self.ot.adc_pipe_data.pop())

    @QtCore.pyqtSlot()
    def emaData(self):
        self.flatc.DecodeObject(self.ot.ema_pipe_data.pop())


class SocketListener(QThread):
    Receiver_Signal = QtCore.pyqtSignal()

    def __init__(self, ot, ff, flatc):
        QThread.__init__(self)
        self.ot = ot
        self.ff = ff
        self.flatc = flatc
        self.Receiver_Signal.connect(self.DecodeData)
        self.start()

    def run(self):
        while True:
            events = self.ff.udp_poll.poll(self.ff.poll_timeout)
            for self.ff.udp_fd, event in events:
                if event & select.POLLIN:
                    # print "Got data in pipe"
                    self.b = os.read(self.ff.udp_conn.fileno(), self.ff.max_fifo_size)
                    self.ot.udp_data.append(self.b)
                    self.Receiver_Signal.emit()
                else:
                    pass
                    #print "Time Out"

    @QtCore.pyqtSlot()
    def DecodeData(self):
        self.flatc.DecodeUdpPacket(self.ot.udp_data.pop())

