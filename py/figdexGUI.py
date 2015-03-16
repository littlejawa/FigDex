# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '../C/qt/FigDexGUI/displaywindow.ui'
#
# Created: Mon Mar 16 22:14:35 2015
#      by: PyQt4 UI code generator 4.10.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_DisplayWindow(object):
    def setupUi(self, DisplayWindow):
        DisplayWindow.setObjectName(_fromUtf8("DisplayWindow"))
        DisplayWindow.resize(400, 318)
        self.centralWidget = QtGui.QWidget(DisplayWindow)
        self.centralWidget.setObjectName(_fromUtf8("centralWidget"))
        self.boxNb = QtGui.QSpinBox(self.centralWidget)
        self.boxNb.setGeometry(QtCore.QRect(110, 10, 54, 23))
        self.boxNb.setMinimum(3)
        self.boxNb.setMaximum(100)
        self.boxNb.setObjectName(_fromUtf8("boxNb"))
        self.lblNb = QtGui.QLabel(self.centralWidget)
        self.lblNb.setGeometry(QtCore.QRect(10, 10, 101, 16))
        self.lblNb.setObjectName(_fromUtf8("lblNb"))
        self.btnStart = QtGui.QPushButton(self.centralWidget)
        self.btnStart.setGeometry(QtCore.QRect(10, 30, 51, 27))
        self.btnStart.setObjectName(_fromUtf8("btnStart"))
        self.lstResults = QtGui.QListWidget(self.centralWidget)
        self.lstResults.setGeometry(QtCore.QRect(10, 60, 161, 251))
        self.lstResults.setObjectName(_fromUtf8("lstResults"))
        DisplayWindow.setCentralWidget(self.centralWidget)

        self.retranslateUi(DisplayWindow)
        QtCore.QMetaObject.connectSlotsByName(DisplayWindow)

    def retranslateUi(self, DisplayWindow):
        DisplayWindow.setWindowTitle(_translate("DisplayWindow", "Calculateur de FIgures Dexiennes", None))
        self.lblNb.setText(_translate("DisplayWindow", "Nombre de points", None))
        self.btnStart.setText(_translate("DisplayWindow", "Lancer", None))

