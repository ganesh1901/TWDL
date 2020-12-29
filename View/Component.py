from PyQt4 import QtGui
from PyQt4 import QtCore
from VarList import  StyleSheet

class Component:
    def __init__(self):
        # type: (object) -> object
        """

        :type st: object
        """
        self.st = StyleSheet()

    def createframe(self, width=-1, height=-1, stylesheet=-1, framestyle=-1):
        frame = QtGui.QFrame()
        frame.setLineWidth(2)
        frame.setMidLineWidth(1)

        if framestyle != -1:
            frame.setFrameStyle(QtGui.QFrame.Box | QtGui.QFrame.Sunken)
        else:
            frame.setFrameStyle(QtGui.QFrame.Box | QtGui.QFrame.Raised)

        if stylesheet != -1:
            frame.setStyleSheet(stylesheet)

        if width != -1:
            frame.setFixedWidth(width)
        if height != -1:
            frame.setFixedHeight(height)
        return frame

    def createlabel(self, width=-1, height=-1, label_name=-1, style=-1):
        label = QtGui.QLabel()
        if width != -1:
            label.setFixedWidth(width)
        if height != -1:
            label.setFixedHeight(height)
        if label_name != -1:
            label.setText(label_name)
        label.setWordWrap(True)
        if style != -1:
            label.setStyleSheet(style)
        return label

    def createlcddisplay(self, width=-11, height=-1, digits=-1):
        # type: (object, object) -> object
        """

        :param width:
        :param height:
        :return:
        :rtype: object
        """
        lcd = QtGui.QLCDNumber()
        if width != -1:
            lcd.setFixedWidth(width)
        if height != -1:
            lcd.setFixedHeight(height)
        if digits != -1:
            lcd.setNumDigits(digits)
        lcd.setFrameShape(QtGui.QFrame.NoFrame)

        return lcd

    def creategroupbox(self, width=-1, height=-1, name=-1):
        groupbox = QtGui.QGroupBox()
        if width != -1:
            groupbox.setFixedWidth(width)
        if height != -1:
            groupbox.setFixedHeight(height)
        if name != -1:
            groupbox.setTitle(name)

        groupbox.setStyleSheet(self.st.gbox)
        return groupbox

    def createlineedit(self, width=-1, height=-1, disable=-1, style=-1):
        line = QtGui.QLineEdit()

        if width != -1:
            line.setFixedWidth(width)

        if height != -1:
            line.setFixedHeight(height)

        if disable != -1:
            line.setDisabled(True)
            line.setStyleSheet(self.st.line_edit_readonly)

        if style != -1:
            line.setStyleSheet(self.st.gbox)

        return line

    def createpushbutton(self, width=-1, height=-1, name=-1):
        button = QtGui.QPushButton()
        if width != -1:
            button.setFixedHeight(height)
        if height != -1:
            button.setFixedWidth(width)
        if name != -1:
            button.setText(name)

        button.setStyleSheet(self.st.gbox)
        return button

    def createcombobox(self, width=-1, height=-1):
        combo = QtGui.QComboBox()
        if width != -1:
            combo.setFixedWidth(width)
        if height != -1:
            combo.setFixedHeight(height)
        return combo

    def createTextEdit(self, width=-1, height=-1, editable=-1, style=-1):
        text = QtGui.QTextEdit()
        if width != -1:
            text.setFixedWidth(width)
        if height != -1:
            text.setFixedHeight(height)
        if editable != -1:
            text.setDisabled(True)
        if style != -1:
            text.setStyleSheet(style)

        return text

    def createRadioButton(self, text=-1, flag=-1):
        rb = QtGui.QRadioButton()
        rb.setText(text)
        rb.setChecked(flag)
        return rb

    def getLabel(self, name, width, height):
        label = QtGui.QLabel()

        label.setWordWrap(True)
        if name != -1:
            label.setText(name)

        if width != -1:
            label.setFixedWidth(width)
        if height != -1:
            label.setFixedHeight(height)

        return label

    def getButton(self, name, width, height):
        btn = QtGui.QPushButton()
        btn.setStyleSheet(self.st.push_button)
        if width != -1:
            btn.setFixedWidth(width)
        if height != -1:
            btn.setFixedHeight(height)
        btn.setText(name)
        return btn

    def getLineEdit(self, flag, width, height):
        te = QtGui.QLineEdit()

        if flag == False:
            te.setEnabled(flag)
            te.setStyleSheet(self.st.line_edit_readonly)

        if height != -1:
            te.setFixedHeight(height)
        if width != -1:
            te.setFixedWidth(width)
        te.setAlignment(QtCore.Qt.AlignCenter)
        return te

    def getTextEdit(self, width, height):
        te = QtGui.QTextEdit()
        if height != -1:
            te.setFixedHeight(height)
        if width != -1:
            te.setFixedWidth(width)
        return te

    def getFrame(self, style, width, height):
        frame = QtGui.QFrame()
        if width != -1:
            frame.setFixedWidth(width)

        if height != -1:
            frame.setFixedHeight(height)

        if style == "Sunken":
            frame.setFrameStyle(QtGui.QFrame.Box | QtGui.QFrame.Sunken)
        else:
            frame.setFrameStyle(QtGui.QFrame.Box | QtGui.QFrame.Raised)

        frame.setLineWidth(1)
        frame.setMidLineWidth(1)
        return frame

    def getRadioButton(self, text, flag):
        rb = QtGui.QRadioButton()
        rb.setText(text)

        rb.setChecked(flag)
        return rb

    def getCheckBox(self, text):
        cb = QtGui.QCheckBox()
        cb.setText(text)

        return cb

    def getGroupBox(self, title, width, height):
        gb = QtGui.QGroupBox()
        gb.setTitle(title)
        gb.setStyleSheet(self.st.gbox)
        if width != -1:
            gb.setFixedWidth(width)
        if height != -1:
            gb.setFixedHeight(height)

        return gb

    def getLcdDisplay(self, width, height, digits):
        lcd = QtGui.QLCDNumber()
        if width != -1:
            lcd.setFixedWidth(width)
        if height != -1:
            lcd.setFixedHeight(height)
        if digits != -1:
            lcd.setNumDigits(digits)
        lcd.setFrameShape(QtGui.QFrame.NoFrame)

        return lcd

    def getCombo(self, width, height):
        combo = QtGui.QComboBox()
        if width != -1:
            combo.setFixedWidth(width)
        if height != -1:
            combo.setFixedHeight(height)
        combo.addItem('Select')
        return combo

    def getCheckBox(self, text, width, height):
        cb = QtGui.QCheckBox()
        cb.setText(text)
        if (width != -1):
            cb.setFixedWidth(width)
        if height != -1:
            cb.setFixedHeight(height)
        cb.setFont(QtGui.QFont("Georgia", 15, QtGui.QFont.Bold))
        return cb

    def getRadioButton(self, text, flag, width, height):
        rb = QtGui.QRadioButton()
        rb.setText(text)
        if (width != -1):
            rb.setFixedWidth(width)
        if (height != -1):
            rb.setFixedHeight(height)
        rb.setFont(QtGui.QFont("Georgia", 15, QtGui.QFont.Bold))
        rb.setChecked(flag)
        return rb

    def getComboBox(self, item_list, width=0, height=0):
        # print 'list length ', len(item_list), '  list ', item_list
        combo = QtGui.QComboBox()
        combo.setFixedHeight(height)
        combo.setFixedWidth(width)
        len1 = len(item_list)
        for i in range(len1):
            combo.addItem(str(item_list[i]))
        return combo


    def createVSpliter(self):
        splitter1 = QtGui.QSplitter()
        splitter1.setOrientation(QtCore.Qt.Vertical)
        return splitter1

    def createHSpliter(self):
        splitter1 = QtGui.QSplitter()
        splitter1.setOrientation(QtCore.Qt.Horizontal)
        return splitter1





