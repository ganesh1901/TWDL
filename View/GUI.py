
from PyQt4 import QtCore,QtGui
import datetime
import os
import sys
from LoginPage import LoginPage
from VarList import FIFO
from VarList import Others
from VarList import StyleSheet
from pyflatc import FlatcOperations
from VarList import TWDL
from Listener import ReceiverHandler
from Component import Component
from Listener import SocketListener


class mainClass:
    def __init__(self):
        self.ot = Others()
        self.ot.logDebug(" Applicatuion Started !!!!")
        self.st = StyleSheet()
        self.ff = FIFO()
        self.twdl = TWDL()
        self.flatc = FlatcOperations(self.ot, self.ff, self.twdl)

        self.ot.app = QtGui.QApplication([self.ot.app_name])

        self.ot.desktop_width = self.ot.app.desktop().geometry().width() - 100
        self.ot.desktop_height = self.ot.app.desktop().geometry().height() - 200
        ret = GUI(self.ot, self.ff, self.st, self.flatc, self.twdl)

        ret1 = LoginPage(self.ot.widget, self.ot, self.ff, self.st, self.flatc, self.twdl)

        self.initListener()
        sys.exit(self.ot.app.exec_())

    def initListener(self):
        # receiver object has to alive otherwise  variable scope problem will arise
        self.rt = ReceiverHandler(self.ot, self.ff, self.flatc)
        self.stt = SocketListener(self.ot, self.ff, self.flatc)


class GUI(QtGui.QMainWindow):

    def __init__(self, ot, ff, st, flatc, twdl):
        super(GUI, self).__init__()
        self.ot = ot
        self.ff = ff
        self.st = st
        self.flatc = flatc
        self.comp = Component()
        self.twdl = twdl
        self.initUI()

    def initUI(self):
        #--- close button only hint --
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint )
        self.resize(self.ot.desktop_width, self.ot.desktop_height)
        self.setWindowTitle(self.ot.app_name)
        self.setCentralWidget(QtGui.QWidget())
        self.ot.widget = self.centralWidget()
        self.show()

        bar = self.menuBar()
        help = bar.addMenu("Help")
        about = bar.addMenu("About")
        screenshot = bar.addMenu("&ScreenShot")

        self.statusbar = self.statusBar()
        self.statusbar.setStyleSheet(self.st.statusbar)
        label1 = self.comp.createlabel(self.ot.desktop_width * 0.1, self.statusbar.height(), "Controller_health", self.st.valid_label)
        self.ot.HStatus = label1
        self.ot.statusbar = self.comp.createlabel(self.ot.desktop_width * 0.6, self.statusbar.height(), '', self.st.valid_label)
        label2 = self.comp.getLabel("Version %s.%s.%s Checksum:%s \n Dated:%s" % (self.ot.MajorNo, self.ot.MinorNo, self.ot.Release, self.ot.checksum, self.ot.dated), self.ot.desktop_width * 0.2, -1)
        label2.setStyleSheet(self.st.valid_label)
        self.ot.notifyStatus("Version %s.%s.%s Checksum:%s \n Dated:%s" % (self.ot.MajorNo, self.ot.MinorNo, self.ot.Release, self.ot.checksum, self.ot.dated), 1)
        self.statusbar.addPermanentWidget(label1)
        self.statusbar.addPermanentWidget(self.ot.statusbar)
        self.statusbar.addPermanentWidget(self.comp.createHSpliter())
        self.statusbar.addPermanentWidget(label2)


        action_shot = QtGui.QAction("ScreenShot", self)
        action_shot.setShortcut("Ctrl+S")
        screenshot.addAction(action_shot)
        screenshot.triggered.connect(self.screenShot)

        action_help = QtGui.QAction("Help", self)
        action_help.setShortcut("Ctrl+H")
        help.addAction(action_help)
        action_help.triggered.connect(self.helpAction)

        action_controller = QtGui.QAction("Controller", self)
        about.addAction(action_controller)
        action_controller.triggered.connect(self.controllerAction)

        action_gui = QtGui.QAction("GUI", self)
        about.addAction(action_gui)
        action_gui.triggered.connect(self.guiAction)

    def gettimedate(self):
        curr_time_object = datetime.datetime.now()
        curr_time = "-%02d-%02d-%04d-%02d-%02d-%02d" % (
            curr_time_object.day, curr_time_object.month, curr_time_object.year, curr_time_object.hour,
            curr_time_object.minute, curr_time_object.second)
        return curr_time

    def screenShot(self):
        self.ot.getScreenShot('../out/test')

    def helpAction(self):
        dialog = QtGui.QDialog()
        dialog.setWindowTitle("Help")
        dialog.exec_()

    def guiAction(self):
        dialog = QtGui.QDialog()
        dialog.setWindowTitle(" ABOUT GUI ")


        dialog.setFixedSize(300, 300)

        gridlayout = QtGui.QGridLayout(dialog)

        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.3, dialog.height() * 0.2, "Version"), 0, 0)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.3, dialog.height() * 0.2, "Date"), 1, 0)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.3, dialog.height() * 0.2, "Checksum"), 2, 0)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.3, dialog.height() * 0.2, "Size"), 3, 0)

        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.4, dialog.height() * 0.2,
                                                "%d.%d.%d" % (self.ot.MajorNo, self.ot.MinorNo, self.ot.Release)), 0, 1)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.4, dialog.height() * 0.2, self.ot.dated), 1, 1)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.4, dialog.height() * 0.2, self.ot.getchecksum()), 2, 1)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.4, dialog.height() * 0.2, self.ot.getappsize()), 3, 1)

        dialog.exec_()


    def controllerAction(self):
        dialog = QtGui.QDialog()
        dialog.setWindowTitle(" ABOUT Controller ")


        dialog.setFixedSize(300, 300)

        gridlayout = QtGui.QGridLayout(dialog)

        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.3, dialog.height() * 0.2, "Version"), 0, 0)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.3, dialog.height() * 0.2, "Date"), 1, 0)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.3, dialog.height() * 0.2, "Size"), 2, 0)

        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.4, dialog.height() * 0.2, self.ot.controller_version), 0, 1)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.4, dialog.height() * 0.2, self.ot.controller_dated), 1, 1)
        gridlayout.addWidget(self.comp.createlabel(dialog.width() * 0.4, dialog.height() * 0.2, self.ot.getControllersize()), 2, 1)


        dialog.exec_()

    def closeEvent(self, event):
        # reply = QtGui.QMessageBox.question(self, 'Message',
        #                                    "Are you sure to quit?", QtGui.QMessageBox.Yes |
        #                                    QtGui.QMessageBox.No, QtGui.QMessageBox.No)
        #
        # if reply == QtGui.QMessageBox.Yes:
        #     event.accept()
        #
        # else:
        #     event.ignore()

        os.system('../run_scripts/kill.sh')
