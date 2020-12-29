from PyQt4 import QtGui, QtCore
import os
import datetime
from subprocess import check_output, CalledProcessError
from Component import Component
from mainapplication import MainApplication


class LoginPage:
    # global variable to access
    timer = QtCore.QTimer()
    status_timer = ''
    lcd_timer = ""

    def __init__(self, mainWindow, ot, ff, st, flatc, twdl):
        # variable initialization
        self.ot = ot
        self.ff = ff
        self.st = st
        self.comp = Component()
        self.flatc = flatc
        self.twdl = twdl

        self.mainWindow = mainWindow

        self.mainWindow.resize(self.mainWindow.width(), self.mainWindow.height())

        vbox = QtGui.QVBoxLayout(self.mainWindow)
        vbox.setContentsMargins(10, 0, 0, 0)

        self.frame1 = self.comp.createframe(self.mainWindow.width() * 0.99, self.mainWindow.height() * 0.1,
                                            self.st.frame, -1)
        self.frame2 = self.comp.createframe(self.mainWindow.width() * 0.99, self.mainWindow.height() * 0.9,
                                            self.st.frame, -1)
        self.frame3 = self.comp.createframe(self.mainWindow.width() * 0.99, self.mainWindow.height() * 0,
                                            self.st.frame, -1)

        vbox.addWidget(self.frame1)
        vbox.addWidget(self.frame2)
        vbox.addWidget(self.frame3)

        # functions calling
        self.header(self.frame1)
        self.body(self.frame2)
        self.footer(self.frame3)

        # testing purpose
        self.ot.stackWidget.setCurrentIndex(1)
        # index.setEnabled(False)

    def Update_Controller_Status(self, a=-1):
        try:
            # print 'alive'
            check_output(["pidof", "simulator"])
            self.ot.HStatus.setText("Controller_Health")
            self.ot.HStatus.setStyleSheet(self.st.valid_label)

        except CalledProcessError:
            self.ot.HStatus.setText("Controller_Health")
            self.ot.HStatus.setStyleSheet(self.st.notvalid_label)
            self.ot.notifyStatus(" Controller Not connected !!!!!", 2)

    def header(self, Pframe):

        self.status_timer = QtCore.QTimer()
        QtCore.QObject.connect(self.status_timer, QtCore.SIGNAL("timeout()"),
                               lambda a=1: self.Update_Controller_Status(a))
        self.status_timer.start(5000)

        vbox11 = QtGui.QHBoxLayout(Pframe)

        logo_label = self.comp.createlabel(Pframe.width() * 0.2, Pframe.height() * 0.5, -1, self.st.header_label)
        logo_label.setPixmap(QtGui.QPixmap('../res/2.jpg'))
        logo_label.setScaledContents(True)

        name_label = self.comp.createlabel(Pframe.width() * 0.5, Pframe.height(),
                                           " TWO WAY DATA LINK SYSTEM \n                   AKASH-NG   ",
                                           self.st.header_label)

        name_label.setStyleSheet(self.st.header_label)

        LoginPage.lcd_timer = self.comp.createlcddisplay(Pframe.width() * 0.3, Pframe.height(), 20)

        LoginPage.timer.timeout.connect(self.Update_Time)
        LoginPage.timer.start(1000)

        sp = QtGui.QSplitter()
        sp.setOrientation(QtCore.Qt.Horizontal)

        # vbox11.addWidget(logo_label)
        vbox11.addWidget(sp)
        vbox11.addWidget(name_label)
        vbox11.addWidget(LoginPage.lcd_timer)

    @staticmethod
    def Update_Time():
        curr_time_object = datetime.datetime.now()
        curr_time = "%02d-%02d-%04d %02d:%02d:%02d" % (curr_time_object.day, curr_time_object.month,
                                                       curr_time_object.year, curr_time_object.hour,
                                                       curr_time_object.minute, curr_time_object.second)

        # print curr_time
        LoginPage.lcd_timer.display(curr_time)

    # layout problem is coming have to check
    def footer(self, Pframe):
       pass

    def body(self, Pframe):
        vbox = QtGui.QVBoxLayout(Pframe)

        stack_widget = QtGui.QStackedWidget()
        stack_widget.setFixedWidth(Pframe.width() * 0.99)
        stack_widget.setFixedHeight(Pframe.height() * 0.98)
        #stack_widget.setFrameStyle(QtGui.QFrame.Box | QtGui.QFrame.Sunken)
        stack_widget.setStyleSheet(self.st.stack_widget)

        self.ot.stackWidget = stack_widget

        loginpage = QtGui.QWidget()
        loginpage.setFixedWidth(Pframe.width())
        loginpage.setFixedHeight(Pframe.height())
        stack_widget.addWidget(loginpage)

        # calling the first login page
        self.login(loginpage)

        # add the application page to stack widget
        page2 = QtGui.QWidget()
        page2.setFixedHeight(Pframe.height())
        page2.setFixedWidth(Pframe.width())
        stack_widget.addWidget(page2)

        # calling the appliction  page2
        #MainApplication(page2, self.ot, self.ff, self.st, self.flatc, self.twdl)
        # adding the stack widget to layout
        vbox.addWidget(stack_widget)

    def login(self, Pframe):
        HBox = QtGui.QHBoxLayout(Pframe)
        HBox.setAlignment(QtCore.Qt.AlignCenter)

        login_gp = self.comp.creategroupbox(Pframe.width() * 0.25, Pframe.height() * 0.5, " Login Details")

        vbox1 = QtGui.QVBoxLayout(login_gp)

        label_list = ["Username", "Password"]

        self.ot.login_list = []
        for i in range(len(label_list)):
            li = []

            hbox = QtGui.QHBoxLayout()

            label = self.comp.createlabel(login_gp.width() * 0.4, login_gp.height() * 0.15, label_list[i], -1)
            hbox.addWidget(label)

            line = self.comp.createlineedit(login_gp.width() * 0.45, login_gp.height() * 0.15, -1, -1)
            # line.setValidator(QtGui.QIntValidator(0, 1e7))
            if (i == 1):
                line.setToolTip(" Password of the user")
                line.setText(self.ot.password)
                line.setEchoMode(QtGui.QLineEdit.Password)
            elif (i == 0):
                line.setToolTip(" Enter the Username")
                line.setText(self.ot.username)

            li.append(label)
            li.append(line)
            self.ot.login_list.append(li)

            hbox.addWidget(line)
            vbox1.addLayout(hbox)

        vbox2 = QtGui.QVBoxLayout()
        vbox2.setAlignment(QtCore.Qt.AlignHCenter | QtCore.Qt.AlignBottom)

        status_label = self.comp.createlabel(Pframe.width(), Pframe.height() * 0.2, -1, -1)
        status_label.hide()

        vbox2.addWidget(status_label)

        vbox1.addLayout(vbox2)

        login_button = self.comp.createpushbutton(login_gp.width() * 0.3, login_gp.height() * 0.1, "Login")
        login_button.setToolTip(" Press button the proceed next")

        QtCore.QObject.connect(login_button, QtCore.SIGNAL("clicked()"),
                               lambda a=login_button, b=self.ot.login_list[0][1], c=self.ot.login_list[1][1],
                                      d=status_label: self.proceed_next(a, b, c, d))

        vbox2.addWidget(login_button)

        details_gp = self.comp.creategroupbox(Pframe.width() * .2, Pframe.height() * 0.7, "PMCS Details")
        self.Make_Checksum_Display(details_gp)

        HBox.addWidget(login_gp)

    # HBox.addWidget(details_gp)

    def Make_Checksum_Display(self, gb):
        vbox = QtGui.QVBoxLayout(gb)
        label_list = ['Version:', 'Dated: ', 'AppCSum: ', 'EXE Size:']
        vbox1 = QtGui.QVBoxLayout()
        obj_list = []
        for i in range(len(label_list)):
            hbox = QtGui.QHBoxLayout()
            label1 = self.comp.createlabel(gb.width() * .4, gb.height() * .2, label_list[i], -1)
            hbox.addWidget(label1)
            label1 = self.comp.createlabel(gb.width() * .5, gb.height() * .2, label_list[i], -1)
            hbox.addWidget(label1)
            obj_list.append(label1)
            vbox1.addLayout(hbox)

        self.Checksum_Details_COMMAND(obj_list)
        vbox.addLayout(vbox1)

    def Checksum_Details_COMMAND(self, obj_list):
        obj_list[0].setText('1.0')
        obj_list[1].setText('24-July-2019')
        cwd = os.getenv('PWD')
        os.system('../run_scripts/checksum.sh')
        obj_list[2].setText(str(self.get_query_value()))
        obj_list[3].setText(str(os.stat('../APP.tar').st_size) + str('  in bytes'))

    def Checkout_Details_REFRESH(self, obj_list):
        for i in obj_list:
            i.setText('')

    def get_query_value(self):
        f = open('/tmp/cfg', 'r')
        buf = ''
        if f:
            buf = f.readline()
            print 'Data', buf
            f.close()
        return buf.split(' ')[0][-8:]

    def proceed_next(self, button_object, username_obj, password_obj, label_obj):
        print ' at callback', button_object, '    ', username_obj.text(), '  ****** ', password_obj.text()
        label_obj.setText("")
        username = username_obj.text()
        password = password_obj.text()

        if username == self.ot.username:
            if password == self.ot.password:
                self.ot.notifyStatus(" Login  Success --!!!! ", 1)
                self.ot.stackWidget.setCurrentIndex(1)
                self.footer(self.frame3)
                os.system("flite -t 'login success '")

            else:
                label_obj.show()
                label_obj.setText(" password not matched ")
                self.ot.notifyStatus(" Password mismatch -- [" + password + ']', 2)
                os.system("flite -t 'login failed'")

        else:
            label_obj.setText(' username not matched')
            self.ot.notifyStatus(" Password mismatch -- [" + username + ']', 2)
