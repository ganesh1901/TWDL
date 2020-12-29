from PyQt4 import QtCore,QtGui
import sys
import pyqtgraph as pg
pg.setConfigOptions(antialias=True)
import time
import random


class Test(QtCore.QThread):
    def __init__(self, widget, app):
        self.widget = widget
        self.app = app
        QtCore.QThread.__init__(self)

        vbox =QtGui.QVBoxLayout(widget)
        self.pyqtgraph = pg.PlotWidget()
        self.pen = pg.mkPen(color=(255, 0, 0))
        self.pen1 = pg.mkPen(color=(0, 255, 0))
        self.pyqtgraph.addLegend()

        self.screen = QtGui.QApplication.primaryScreen()


        button = QtGui.QPushButton()
        button.setText("Clear")
        QtCore.QObject.connect(button, QtCore.SIGNAL("clicked()"), lambda a=1:self.Clear(a))

        self.p1 = self.pyqtgraph.plot([], name='test1', pen=self.pen)
        self.p2 = self.pyqtgraph.plot([], name='test2', pen=self.pen1)


        vbox.addWidget(self.pyqtgraph)
        vbox.addWidget(button)

    def Clear(self, index):
        self.screenshot = self.screen.grabWindow(self.app.desktop().winId())
        self.screenshot.save('../out/2.png')

    def run(self):
        self.c1 = []
        self.c2 = []
        self.c3 = []
        self.count = 0
        while True:
            time.sleep(0.2)
            self.count += 1
            self.c1.append(random.randint(0, 1000))
            self.c2.append(random.randint(-1000, 3000))
            self.c3.append(self.count * 0.0025)
            self.p1.setData(self.c3, self.c1)
            self.p2.setData(self.c3, self.c2)


if __name__ == '__main__':
    app = QtGui.QApplication([])
    widget = QtGui.QWidget()
    t = Test(widget, app)
    t.start()
    widget.show()
    sys.exit(app.exec_())
