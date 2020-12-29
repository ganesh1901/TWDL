from PyQt4 import QtGui,QtCore
import pyqtgraph as pg
import sys
import os
import struct
pg.setConfigOptions(antialias=True)


class testPlot:
    data_set = []
    data_set_x = []
    def __init__(self, widget):
        vbox = QtGui.QVBoxLayout(widget)
        self.pyqtgraph = pg.PlotWidget()
        self.pen = pg.mkPen(color=(255, 0, 0))
        self.pyqtgraph.addLegend()
        self.pyqtgraph.showGrid(x=True, y=True)

        self.pyqtgraph.setTitle(" Acuator ")
        self.pyqtgraph.setLabel('left', "<span style=\"color:red;font-size:20px\"> Degrees(in volts)</span>")
        self.pyqtgraph.setLabel('bottom','<span style="color:red;font-size:20px"> 1sec = 400 samples </span>')

        self.data = self.pyqtgraph.plot([], name="Frame A", pen=self.pen)

        button = QtGui.QPushButton()
        button.setText("Clear")
        vbox.addWidget(self.pyqtgraph)
        vbox.addWidget(button)

        self.run()

    def run(self):
        curr = 0
        prev = 0
        try:
            ff = open(sys.argv[1], "rb")
            l1 = os.stat(sys.argv[1]).st_size / 74
            print 'length 00l', l1
            for ii in range(l1):
                b1 = ff.read(74)
                if b1:
                    d1 = struct.unpack("31hH", b1[10:74])
                    if ii==0:
                        curr = prev
                    elif ii == 40200:
                        break
                    curr = d1[31]
                    diff = curr - prev
                    self.data_set.append(diff)
                    prev = curr
                    self.data_set_x.append(ii * 0.02)

            self.data.setData(self.data_set_x, self.data_set)
        except:
            print 'got error'


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print 'usage python .py Filename '
        sys.exit(0)
    print sys.argv[1]
    app = QtGui.QApplication(["test plot"])
    widget = QtGui.QWidget()
    tt = testPlot(widget)
    widget.show()
    sys.exit(app.exec_())