import sys

import logging

import time

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import (
    QLine,
    QTimer,
    Qt,
    QThreadPool,  
    pyqtSlot
)

from PyQt5.QtWidgets import (
    QApplication,
    QGridLayout,
    QLabel,
    QLineEdit,
    QMainWindow,
    QProgressBar,
    QPushButton,
    QSpinBox,
    QTextEdit,
    QComboBox,
    QHBoxLayout,
    QVBoxLayout,
    QMessageBox,
    QWidget,
    QSpinBox
    
)

from collections import deque

import serial
import serial.tools.list_ports
# We import library dedicated to data plot
import pyqtgraph as pg
import numpy as np
from pyqtgraph import PlotWidget
from serial_worker import CONN_STATUS, SerialWorkerSignals
from random import randint

import serial_worker as wrk

#Headers and Tails for data packaging
H_S = 0xA0
T_S = 0xC0
H_P = 0x0F
T_P = 0xFB
H_R = 0xAA
T_R = 0xBB



###############
# MAIN WINDOW #
###############
class MainWindow(QMainWindow): 
    
    def __init__(self):
       
        """!
        @brief Init MainWindow.
        """
        # define worker
        self.serial_worker = wrk.SerialWorker(None)

        super(MainWindow, self).__init__()

        # title and geometry
        self.setWindowTitle("GUI")
        width = 900
        height = 600
        self.setMinimumSize(width, height)

        # create thread handler
        self.threadpool = QThreadPool()

        self.connected = wrk.CONN_STATUS
        self.serialscan()
        self.initUI()

        #Initialization of variables
        self.Flag_Crono = 0
        self.Flag_P = 0
        self.A_P = 0
        self.Flag_F = 0
        self.APPP = 0
        self.counter_peak = 0
        self.period_value = 1
        self.x_up = list()
        self.y_up = list()
        self.x_down = list()
        self.y_down = list()
        self.x_crono = list()
        self.y_crono = list()
        self.contasec = 0
        self.Early_Stop = 0

    #####################
    # GRAPHIC INTERFACE #
    #####################
    def initUI(self):
        """!
        @brief Set up the graphical interface structure.
        """
        #Create plots for CV and Chrono
        self.graphWidget = PlotWidget()
        self.CronoPlot = PlotWidget()

        #Create various buttons
        self.start_btn = QPushButton(
            text=("Start"),
            enabled = False
        )
        self.start_btn.clicked.connect(lambda state, x=wrk.START_STRM: self.start_stream(state, x))

        self.stop_btn = QPushButton(
            text=("Stop"),
            enabled = False
        )
        self.stop_btn.clicked.connect(lambda state, x=wrk.STOP_STRM: self.stop_stream(state, x))

        self.clear_btn = QPushButton(
            text="Clear",
            checkable=True,
            toggled=self.clear_plot,
            enabled = False
        )

        self.send_btn = QPushButton(
            text = ("Ok"),
            checkable=True,
            toggled=self.choose_voltage,
            enabled = False
        )           

        self.cv_btn = QPushButton(
            text=("Ciclovoltammetry"),
            enabled = False
        )
        self.cv_btn.clicked.connect(lambda state, x=wrk.CV_CH: self.choose_cv(state, x))

        self.crono_btn = QPushButton(
            text=("Cronoamperometry"),
            enabled = True
        )
        self.crono_btn.clicked.connect(lambda state, x=wrk.CRONO_CH: self.choose_crono(state, x))

        self.hundred_btn = QPushButton(
            text=("100 mV/s"),
            enabled = False
        )
        self.hundred_btn.clicked.connect(lambda state, x=wrk.STEP_HUN: self.step_hun(state, x))

        self.fifty_btn = QPushButton(
            text=("50 mV/s"),
            enabled = True
        )
        self.fifty_btn.clicked.connect(lambda state, x=wrk.STEP_FIF: self.step_fif(state, x))

        self.two_el_btn = QPushButton(
            text=("2-electrodes mode"),
            enabled = False
        )
        self.two_el_btn.clicked.connect(lambda state, x=wrk.TWO_EL: self.two_el_set(state, x))

        self.th_el_btn = QPushButton(
            text=("3-electrodes mode"),
            enabled = True
        )
        self.th_el_btn.clicked.connect(lambda state, x=wrk.TH_EL: self.th_el_set(state, x))

        #Create the progress bar
        self.pbar = QProgressBar()

        #Create the boxes to choose the voltages
        self.spinbox = QSpinBox() # -> Positive Potential
        self.spinbox.setMinimum(0)
        self.spinbox.setMaximum(1000)
        self.spinbox.setSingleStep(100)
        self.spinbox.valueChanged.connect(self.value_changed_p)

        self.spinbox2 = QSpinBox() # -> Negative Potential
        self.spinbox2.setMinimum(-1000)
        self.spinbox2.setMaximum(0)
        self.spinbox2.setSingleStep(100)
        self.spinbox2.valueChanged.connect(self.value_changed_n)

        self.spinbox3 = QSpinBox() # -> Chrono potential
        self.spinbox3.setMinimum(-1000)
        self.spinbox3.setMaximum(1000)
        self.spinbox3.setSingleStep(100)
        self.spinbox3.valueChanged.connect(self.value_changed_ch)

        self.spinbox4 = QSpinBox() # -> Chrono potential
        self.spinbox4.setMinimum(0)
        self.spinbox4.setMaximum(20)
        self.spinbox4.setSingleStep(1)
        self.spinbox4.valueChanged.connect(self.value_changed_stop)

        self.period_list = QComboBox()
        self.period_list.addItems(["One Period", "Two Periods"])
        self.period_list.setCurrentIndex(1)
        self.period_list.currentIndexChanged.connect(self.period_changed)

        self.Set_Stop = QComboBox()
        self.Set_Stop.addItems(["No", "Yes"])
        self.Set_Stop.setCurrentIndex(0)
        self.Set_Stop.currentIndexChanged.connect(self.sel_stop)

        self.R_TIA = QComboBox()
        self.R_TIA.addItems(["20", "30", "40", "80", "120", "250", "500", "1000"])
        self.R_TIA.setCurrentIndex(4)
        self.R_TIA.currentIndexChanged.connect(self.R_changed)

    
        #Create the labels that describe the objects
        self.labelA = QLabel()
        self.labelA.setText('Negative potential:')
        self.labelB = QLabel()
        self.labelB.setText('Positive potential:')
        self.labelC = QLabel()
        self.labelC.setText('(-1000 to 0 mV)')
        self.labelD = QLabel()
        self.labelD.setText('(0 to +1000 mV)')
        self.labelE = QLabel()
        self.labelE.setText('Data acquisition progress:')
        self.labelF = QLabel() # -> Positive Peak Voltage [mV]
        self.labelG = QLabel() # -> Negative Peak Voltage [mV]
        self.labelH = QLabel() # -> Positive output
        self.labelI = QLabel() # -> Negative output
        self.labelJ = QLabel() # -> Crono Potential [mV]
        self.labelK = QLabel() # -> Step Potential
        self.labelK.setText('Step Potential:')
        self.labelL = QLabel()
        self.labelL.setText('Measurement on:')
        self.labelM = QLabel()
        self.labelN = QLabel()
        self.labelN.setText('Choose measurement period:')
        self.labelO = QLabel()
        self.labelO.setText('Stop after [s]:')
        self.labelP = QLabel()
        self.labelP.setText('TIA resistance:')
       
        
        #Create the layout
        layout1 = QVBoxLayout() 
        layout2 = QVBoxLayout() 
        layout3 = QVBoxLayout() 
        layout4 = QVBoxLayout() 
        layout5 = QHBoxLayout() 
        layout6 = QVBoxLayout() 
        layout7 = QVBoxLayout() 
        
        layout4.setContentsMargins(0, 50, 0, 180)
        layout6.setContentsMargins(0, 50, 0, 0)
        layout7.setContentsMargins(0, 30 , 0 , 70)

        layout2.addWidget(self.com_list_widget)
        layout2.addWidget(self.conn_btn, alignment= QtCore.Qt.AlignTop)
        layout2.addWidget(self.cv_btn)
        layout2.addWidget(self.crono_btn)
        layout2.addWidget(self.two_el_btn, alignment= QtCore.Qt.AlignBottom)
        layout2.addWidget(self.th_el_btn)
        layout2.addWidget(self.labelP, alignment= QtCore.Qt.AlignBottom)
        layout2.addWidget(self.R_TIA)
        layout4.addWidget(self.start_btn, alignment = QtCore.Qt.AlignBottom)
        layout4.addWidget(self.stop_btn)
        layout4.addWidget(self.clear_btn)

        layout5.addLayout(layout2)
        layout1.addWidget(self.graphWidget)
        layout1.addWidget(self.CronoPlot)
        self.CronoPlot.hide()
        layout1.addWidget(self.labelE)
        self.labelE.hide()
        layout1.addWidget(self.pbar)
        self.pbar.hide()
        layout5.addLayout(layout1)
        
        layout3.addWidget(self.labelA, alignment = QtCore.Qt.AlignBottom)
        layout3.addWidget(self.spinbox2)
        layout3.addWidget(self.labelC, alignment = QtCore.Qt.AlignTop)
        layout3.addWidget(self.labelB, alignment = QtCore.Qt.AlignBottom)
        layout3.addWidget(self.spinbox)
        layout3.addWidget(self.labelD, alignment = QtCore.Qt.AlignTop)
        layout3.addWidget(self.labelL, alignment = QtCore.Qt.AlignBottom)
        layout3.addWidget(self.period_list)
        layout3.addWidget(self.labelK, alignment = QtCore.Qt.AlignBottom)
        layout3.addWidget(self.hundred_btn)
        layout3.addWidget(self.fifty_btn)
        layout3.addWidget(self.labelJ, alignment = QtCore.Qt.AlignBottom)
        self.labelJ.hide()
        layout3.addWidget(self.spinbox3)
        self.spinbox3.hide()
        layout3.addWidget(self.labelN, alignment = QtCore.Qt.AlignBottom)
        self.labelN.hide()
        layout3.addWidget(self.Set_Stop)
        self.Set_Stop.hide()
        layout3.addWidget(self.labelO)
        self.labelO.hide()
        layout3.addWidget(self.spinbox4, alignment = QtCore.Qt.AlignTop)
        self.spinbox4.hide()
        layout3.addWidget(self.labelM, alignment = QtCore.Qt.AlignCenter)
        self.labelM.hide()
       
        layout6.addWidget(self.send_btn, alignment = QtCore.Qt.AlignTop)

        layout7.addWidget(self.labelF, alignment = QtCore.Qt.AlignCenter)
        layout7.addWidget(self.labelH, alignment = QtCore.Qt.AlignCenter)
        layout7.addWidget(self.labelG, alignment = QtCore.Qt.AlignCenter)
        layout7.addWidget(self.labelI, alignment = QtCore.Qt.AlignCenter)

        layout2.addLayout(layout4)
        layout3.addLayout(layout6)
        layout3.addLayout(layout7)
        layout5.addLayout(layout3)
        
        widget = QWidget()
        widget.setLayout(layout5)
        self.setCentralWidget(widget)
 
         # CV plot settings
        self.graphWidget.showGrid(x=True, y=True)
        self.graphWidget.setBackground('w')
        self.graphWidget.setTitle("Albumin Measurement",  color ="k", size = "20pt" )
        styles = {'color':'r', 'font-size':'20px'}
        self.graphWidget.setLabel('left', "<span style=\"color:black;font-size:12px\">Current (nA)</span>")
        self.graphWidget.setLabel('bottom', "<span style=\"color:black;font-size:12px\">Voltage (mV)</span>")

        #Chrono plot setting
        self.CronoPlot.showGrid(x=True, y=True)
        self.CronoPlot.setBackground('w')
        self.CronoPlot.setTitle("Albumin Measurement",  color ="k", size = "20pt" )
        styles = {'color':'r', 'font-size':'20px'}
        self.CronoPlot.setLabel('left', "<span style=\"color:black;font-size:12px\">Current (nA)</span>")
        self.CronoPlot.setLabel('bottom', "<span style=\"color:black;font-size:12px\">Time (s)</span>")
    
    def value_changed_p(self, i):
        self.Positive_potential= i

    def value_changed_n(self, j):
        self.Negative_pontial= j
    
    def value_changed_ch(self, z):
        self.Chrono_potential= z

    def value_changed_stop(self, k):
        self.stop_sec = k

    def period_changed(self, h):
        self.period_value = int(h)
        self.data_period = [k for k in range(0, 4)]
        self.data_period[0] = H_P
        self.data_period[1] = self.period_value >> 8
        self.data_period[2] = self.period_value & 0xFF   
        self.data_period[3] = T_P

        self.serial_worker.port.write(self.data_period)
        logging.info("Written {} on port {}.".format(self.data_period[2], self.serial_worker.port_name))

    def R_changed(self, h):
        self.R_value = int(h)
        self.data_R = [k for k in range(0, 4)]
        self.data_R[0] = H_R
        self.data_R[1] = self.R_value >> 8
        self.data_R[2] = self.R_value & 0xFF   
        self.data_R[3] = T_R

        self.serial_worker.port.write(self.data_R)
        logging.info("Written {} on port {}.".format(self.data_R[2], self.serial_worker.port_name))

    def clear_plot(self, checked):
        if checked:
            self.graphWidget.clear()
            self.CronoPlot.clear()
            self.clear_btn.setChecked(False)
    
        
    ####################
    # SERIAL INTERFACE #
    ####################
    def serialscan(self):
        """!
        @brief Scans all serial ports and create a list.
        """
        #Create the combo box to host port list
        self.port_text = ""
        self.com_list_widget = QComboBox()
        self.com_list_widget.currentTextChanged.connect(self.port_changed)

        #Create the connection button
        self.conn_btn = QPushButton(
            text=("Connect to port {}".format(self.port_text)), 
            checkable=True,
            toggled=self.on_toggle
        )

        #Acquire list of serial ports, add it to the combo box and
        #choose the one that PSoC is connected to
        serial_ports = [
                p.name
                for p in serial.tools.list_ports.comports()
                if 'KitProg USB-UART' in p.description
            ]
        
        if not serial_ports:
            raise IOError("No PSoC found - is it plugged in? If so, restart computer.")
        if len(serial_ports) > 1:
            #warnings.warn('Multiple PSOCs found - using the first')
            msg = QMessageBox()
            msg.setIcon(QMessageBox.Information)
            msg.setText("Multiple PSOCs found - using the first")
            #msg.setInformativeText('More information')
            msg.setWindowTitle("Error")
            msg.exec_()

        self.com_list_widget.addItems(serial_ports)


    ##################
    # SERIAL SIGNALS #
    ##################
    def port_changed(self):
        """!
        @brief Update conn_btn label based on selected port.
        """
        self.port_text = self.com_list_widget.currentText()
        self.conn_btn.setText("Connect to port {}".format(self.port_text))

    @pyqtSlot(bool)
    def on_toggle(self, checked):
        """!
        @brief Allow connection and disconnection from selected serial port and check if PSoC is correctly programmed.
        """
        if checked:
            # setup reading worker
            self.serial_worker = wrk.SerialWorker(self.port_text) # needs to be re defined
            self.serial_worker.is_streaming = True
            # execute the worker
            self.threadpool.start(self.serial_worker)
            time.sleep(0.5)
            self.programmed()
            
        else:
            # kill thread
            self.serial_worker.is_streaming = False
            self.serial_worker.is_killed = True
            self.com_list_widget.setDisabled(False) # enable the possibility to change port
            self.conn_btn.setText(
                "Connect to port {}".format(self.port_text)
            )

    def programmed(self):
        self.serial_worker.signals.data.connect(self.programmed_correctly)
        self.serial_worker.send('p')

    def programmed_correctly(self, data):
        if data == -1:
            self.Flag_P = 1
            self.serial_worker.signals.status.connect(self.check_serialport_status)
            self.serial_worker.signals.device_port.connect(self.connected_device)
            self.com_list_widget.setDisabled(True) # disable the possibility to change COM port when already connected
            self.conn_btn.setText(
                "Disconnect from port {}".format(self.serial_worker.port_name)
            )
            logging.info("Connected to port {}".format(self.serial_worker.port_name))
            self.send_btn.setEnabled(True)

   
    def check_serialport_status(self, port_name, status):
        """!
        @brief Handle the status of the serial port connection.

        Available status:
            - 0  --> Error during opening of serial port
            - 1  --> Serial port opened correctly
            - 2  --> Error during reading
        """
        if status == 0:
            self.conn_btn.setChecked(False)

        elif status == 2:            
            self.conn_btn.setChecked(False)
            self.on_btn.setDisabled(False)
            self.off_btn.setDisabled(False)
            self.serial_worker.is_streaming = False
            dlg = QMessageBox(self)
            dlg.setWindowTitle("Warning")
            dlg.setText("Cannot communicate with port {}. Please check the connection and try again.".format(port_name))
            dlg.setStandardButtons(QMessageBox.Ok)
            dlg.setIcon(QMessageBox.Warning)
            button = dlg.exec_()            

    def connected_device(self, port_name):
        """!
        @brief Checks on the termination of the serial worker.
        """
        logging.info("Port {} closed.".format(port_name))

    
    
    @pyqtSlot(bool)
    def choose_voltage(self, checked):
        """!
        @brief Send to PSoC the set potential values
        """
        self.step_crono = 0

        if checked:
            if self.Flag_Crono == 0: 
                self.length = int((self.Positive_potential - self.Negative_pontial)/3)
                self.serial_worker.send_voltage(self.Positive_potential, self.Negative_pontial)
            if self.Flag_Crono == 1:
                if self.Early_Stop == 0:
                    self.serial_worker.signals.data.connect(self.handle_data_crono)
                    self.serial_worker.send_crono(self.Chrono_potential)
                elif self.Early_Stop == 1:
                    self.serial_worker.signals.data.connect(self.early_stop)
                    self.serial_worker.send_crono(self.Chrono_potential)


            self.start_btn.setEnabled(True)
            self.stop_btn.setEnabled(True)
            self.clear_btn.setEnabled(True) 
            self.send_btn.setChecked(False)
            self.send_btn.setEnabled(False) 
      

    #########################
    # READ WORKER FUNCTIONS #
    #########################
    def count_sec(self):
        """!
        @brief Count the seconds of the progress bar
        """
        if self.timer_counter == 101:
            self.timer.stop()
            self.timer_counter = 0
            self.pbar.hide()
            self.labelE.hide()
        self.pbar.setValue(self.timer_counter)
        self.timer_counter += 1
        
    def start_stream(self, state, char):
        """!
        @brief Handle start data stream.

        @param state is the state of the button, always False since this button is not checkable.
        @param char is the char to be sent on serial port
        """
        if self.Flag_Crono == 0:
            self.start_CV()
            
        if self.Flag_Crono == 1:
            self.start_Chrono()

        self.serial_worker.send(char)
        self.start_btn.setEnabled(False)
        self.labelF.hide()
        self.labelG.hide()
        self.labelH.hide()
        self.labelI.hide()

    def start_CV(self):
        self.graphWidget.clear()
        pen = pg.mkPen(color=(255, 0, 0))
        pen2 = pg.mkPen(color=(0, 255, 0))
        self.data_line = self.graphWidget.plot(self.x_up, self.y_up, pen=pen)
        self.data_line2 = self.graphWidget.plot(self.x_down, self.y_down, pen=pen2)
        self.counter = 0
        self.counter_down = 0
        self.counter_peak = 0
        self.old_up = 0
        self.old_down = 0
        self.step_size = self.Negative_pontial
        self.timer_counter = 0
        self.labelE.show()
        self.pbar.show()
        self.timer = QTimer()
        self.timer.timeout.connect(self.count_sec)
        if (self.Flag_F == 0) & (self.period_value == 1):
            self.perc = int(((self.Positive_potential - self.Negative_pontial)/3)*4)-1
        elif (self.Flag_F == 0) & (self.period_value == 0):
            self.perc = int(((self.Positive_potential - self.Negative_pontial)/3)*2)-1

        elif (self.Flag_F == 1) & (self.period_value == 1):
            self.perc = int(((self.Positive_potential - self.Negative_pontial)/3)*8)-1
        elif (self.Flag_F == 1) & (self.period_value == 0):
            self.perc = int(((self.Positive_potential - self.Negative_pontial)/3)*4)-1
        self.percen = int(self.perc * 0.29)
        self.timer.start(self.percen)
        self.serial_worker.signals.data.connect(self.handle_data)

    def start_Chrono(self):
        self.CronoPlot.clear()
        pen = pg.mkPen(color=(255, 0, 0))
        self.data_line_crono = self.CronoPlot.plot(self.x_crono, self.y_crono, pen=pen)
        self.step_crono = 0
        self.x_crono.clear()
        self.y_crono.clear()

            
    def stop_stream(self, state, char):
        """!
        @brief Handle stop data stream.

        @param state is the state of the button, always False since this button is not checkable.
        @param char is the char to be sent on serial port
        """
        if self.Flag_Crono == 0:
            self.timer.stop()
            self.pbar.hide()
            self.labelE.hide()
            self.counter = 0
            self.counter_down = 0
            self.old_up = 0
            self.old_down = 0
            self.x_up.clear()
            self.y_up.clear()
            self.x_down.clear()
            self.y_down.clear()
            self.counter_peak = 0
        if self.Flag_Crono == 1:
            self.contasec = 0
            self.labelM.setText("Current value [nA]:" + str((self.y_crono[-1])))
        self.stop_btn.setEnabled(False)
        self.send_btn.setEnabled(True)
        self.serial_worker.send(char)
        self.serial_worker.flush_port()
        time.sleep(0.1)
        self.conn_btn.setChecked(False)
        time.sleep(0.1)
        self.conn_btn.setChecked(True)
        
    def handle_data(self, data):
        """!
        @brief Update output_window
        """
        if self.period_value == 1:
            self.handle_two_periods(data)
        
        elif self.period_value == 0:
            self.handle_one_period(data)

    def handle_two_periods(self, data):
        if  self.counter < self.length:
            self.x_up.append(self.step_size)
            self.y_up.append(data)
            self.data_line.setData(self.x_up, self.y_up)
            self.step_size += 3
            if self.counter == self.length:
                self.step_size = self.step_size - 3
            self.counter += 1
        
        elif self.counter_down < self.length-1: 
             self.x_down.append(self.step_size)
             self.y_down.append(data)
             self.data_line2.setData(self.x_down, self.y_down)
             self.step_size -= 3
             self.counter_down +=1

        if self.counter_down == self.length-1:
            self.send_btn.setEnabled(True)
            self.start_btn.setEnabled(True)

        self.counter_peak += 1

        if self.counter_peak == 2*self.length:
            self.peak_Ox(data)
        if self.counter_peak == 2*self.length + 1:
            self.peak_Red(data)

    def handle_one_period(self, data):   
        if self.counter < self.length:
           self.x_up.append(self.step_size)
           self.y_up.append(data)
           self.data_line.setData(self.x_up, self.y_up)
           self.step_size += 3
           if self.counter == self.length:
               self.step_size = self.step_size - 3
           self.counter += 1
        
        elif self.counter_down < self.length-1: 
             self.x_down.append(self.step_size)
             self.y_down.append(data)
             self.data_line2.setData(self.x_down, self.y_down)
             self.step_size -= 3
             self.counter_down +=1

        if self.counter_down == self.length-1:
            self.send_btn.setEnabled(True)
            self.start_btn.setEnabled(True)

        self.counter_peak += 1

        if self.counter_peak == 2*self.length:
            self.peak_Ox(data)
        if self.counter_peak == 2*self.length + 1:
            self.peak_Red(data)         

    def peak_Ox(self, data):
        self.data_max = data
        self.labelF.show()
        self.labelF.setText('Positive Peak Voltage [mV]')
        self.labelH.setText(str(self.data_max))
        self.labelH.show()
        
    def peak_Red(self, data):
        self.data_min = data
        self.labelG.show()
        self.labelG.setText('Negative Peak Voltage [mV]')
        self.labelI.show()
        self.labelI.setText(str(self.data_min))
        self.serial_worker.flush_port()
        self.x_up.clear()
        self.y_up.clear()
        self.y_down.clear()
        self.x_down.clear()
        self.conn_btn.setChecked(False)
        time.sleep(0.1)
        self.conn_btn.setChecked(True)


    def handle_data_crono(self, data):
        self.x_crono.append(self.step_crono)
        self.y_crono.append(data)
        self.data_line_crono.setData(self.x_crono, self.y_crono)
        self.step_crono += 0.03
        #self.contasec += 1 
        #if self.contasec >= 166:
         #   self.stop_prima()

    def early_stop(self, data):
        self.x_crono.append(self.step_crono)
        self.y_crono.append(data)
        self.data_line_crono.setData(self.x_crono, self.y_crono)
        self.step_crono += 0.03
        self.contasec += 1

        if self.contasec >= 33.3*self.stop_sec:
            self.serial_worker.send(wrk.STOP_STRM)
            self.contasec = 0
            self.labelM.setText("Current value [nA]:" + str((self.y_crono[-1])))
            self.stop_btn.setEnabled(False)
            self.send_btn.setEnabled(True)
            self.serial_worker.flush_port()
            time.sleep(0.1)
            self.conn_btn.setChecked(False)
            time.sleep(0.1)
            self.conn_btn.setChecked(True)
            

    def choose_cv(self, state, char):
        self.Flag_Crono = 0
        self.CronoPlot.clear()
        self.CronoPlot.hide()
        self.graphWidget.show()
        self.graphWidget.setYRange(3000, -3000, 0)
        self.crono_btn.setEnabled(True)
        self.cv_btn.setEnabled(False)
        self.labelJ.hide()
        self.spinbox3.hide()
        self.labelN.hide()
        self.Set_Stop.hide()
        self.labelO.hide()
        self.spinbox4.hide()
        self.labelA.show()
        self.spinbox2.show()
        self.labelB.show()
        self.spinbox.show()
        self.labelC.show()
        self.labelD.show() 
        self.labelE.hide()
        self.labelL.show()
        self.period_list.show()
        self.labelK.show()
        self.hundred_btn.show()
        self.fifty_btn.show()
        self.pbar.hide()
        self.labelM.hide()
        self.serial_worker.send(char)

    def choose_crono(self, state, char):
        self.Flag_Crono = 1
        self.graphWidget.clear()
        self.graphWidget.hide()
        self.CronoPlot.setYRange(3000, -3000, 0)
        self.CronoPlot.show()
        self.cv_btn.setEnabled(True)
        self.crono_btn.setEnabled(False)
        self.labelA.hide()
        self.spinbox2.hide()
        self.labelB.hide()
        self.spinbox.hide()
        self.labelC.hide()
        self.labelD.hide()
        self.labelE.hide()
        self.labelL.hide()
        self.period_list.hide()
        self.pbar.hide()
        self.labelK.hide()
        self.hundred_btn.hide()
        self.fifty_btn.hide()
        self.labelJ.show()
        self.labelJ.setText('Crono Potential [mV]')
        self.spinbox3.show()
        self.Set_Stop.show()
        self.labelN.show()
        self.labelM.show()
        self.serial_worker.send(char)

    def step_hun(self, state, char):
        self.fifty_btn.setEnabled(True)
        self.hundred_btn.setEnabled(False)
        self.Flag_F = 0
        self.serial_worker.send(char)
    def step_fif(self, state, char):
        self.hundred_btn.setEnabled(True)
        self.fifty_btn.setEnabled(False)
        self.Flag_F = 1
        self.serial_worker.send(char)

    def two_el_set(self, state, char):
        self.th_el_btn.setEnabled(True)
        self.two_el_btn.setEnabled(False)
        self.serial_worker.send(char)
    def th_el_set(self, state, char):
        self.two_el_btn.setEnabled(True)
        self.th_el_btn.setEnabled(False)
        self.serial_worker.send(char)

    
    def sel_stop(self, sel):
        if sel == 0:
            self.Early_Stop = 0
            self.labelO.hide()
            self.spinbox4.hide()
        elif sel == 1:
            self.Early_Stop = 1
            self.labelO.show()
            self.spinbox4.show()



    #######
    # AUX #
    #######
        
    def ExitHandler(self):
        """!
        @brief Kill every possible running thread upon exiting application.
        """
        if wrk.CONN_STATUS:
            self.serial_worker.send(wrk.STOP_STRM)        
            self.serial_worker.is_streaming = False
            self.serial_worker.is_killed = True
    
#############
#  RUN APP  #
#############
if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MainWindow()
    app.aboutToQuit.connect(w.ExitHandler)
    w.show()
    sys.exit(app.exec_())