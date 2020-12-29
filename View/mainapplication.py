
from PyQt4 import QtGui
from TWDL import TWDLWindow


class MainApplication:
    def __init__(self, widget, ot, ff, st, flatc, twdl):
        self.mainWindow = widget
        self.ot = ot
        self.st = st
        self.ff = ff
        self.flatc = flatc
        self.twdl = twdl

        VBOX = QtGui.QVBoxLayout(self.mainWindow)
        widget = QtGui.QWidget()
        widget.setFixedSize(self.mainWindow.width() * 0.98, self.mainWindow.height() * 0.95)
        TWDLWindow(widget, self.ot, self.flatc, self.ff, self.twdl)
        VBOX.addWidget(widget)









