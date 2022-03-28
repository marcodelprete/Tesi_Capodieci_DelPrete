import time

import struct

import logging

from PyQt5.QtCore import (
    QObject, 
    QRunnable, 
    pyqtSignal, 
    pyqtSlot
)
import numpy as np
import serial
import serial.tools.list_ports


# Globals
CONN_STATUS = False

#Characters for communication with PSoC 
START_STRM = "v"
STOP_STRM = "f"
CV_CH = "c"
CRONO_CH = "r"
STEP_HUN = "h"
STEP_FIF = "s"
TWO_EL = "w"
TH_EL = "e"

#Headers and Tails for data packaging 
HEADER = 0x0A
TAIL = 0xFF
H_C = 0x0D 
T_C = 0xFE
H_CN = 0x0E
T_CN = 0xFD
H_CV = 0xA0
T_CV = 0xC0

# Logging config
logging.basicConfig(format="%(message)s", level=logging.INFO)


#########################
# SERIAL_WORKER_SIGNALS #
#########################
class SerialWorkerSignals(QObject):
    """!
    @brief Class that defines the signals available to a serialworker.

    Available signals (with respective inputs) are:
        - device_port:
            str --> port name to which a device is connected
        - status:
            str --> port name
            int --> macro representing the state (0 - error during opening, 1 - success, 2 - reading error)    
        - data:
            list  --> current values to be plotted      
    """
    device_port = pyqtSignal(str)
    status = pyqtSignal(str, int)
    data = pyqtSignal(int)
    #prog = pyqtSignal(int)


#################
# SERIAL_WORKER #
#################
class SerialWorker(QRunnable):
    """!
    @brief Main class for serial communication: handles connection with device.
    """
    def __init__(self, serial_port_name):
        """!
        @brief Init worker.
        """
        self.is_killed = False
        self.is_streaming = False
        self.read_state = 0
        super().__init__()
        # init port, params and signals
        self.port = serial.Serial()
        self.port_name = serial_port_name
        self.baudrate = 115200 # hard coded but can be a global variable, or an input param
        self.signals = SerialWorkerSignals()

    @pyqtSlot()
    def run(self):
        """!
        @brief Estabilish connection with desired serial port.
        """
        global CONN_STATUS

        if not CONN_STATUS:
            try:
                self.port = serial.Serial(port=self.port_name, baudrate=self.baudrate,
                                        write_timeout=0, timeout=2)                
                if self.port.is_open:
                    CONN_STATUS = True
                    self.signals.status.emit(self.port_name, 1)
                    time.sleep(0.01)     
            except serial.SerialException:
                logging.info("Error with port {}.".format(self.port_name))
                self.signals.status.emit(self.port_name, 0)
                time.sleep(0.01)

            while(self.is_streaming):
                if self.port.is_open:
                    try:
                        if self.port.in_waiting > 0:
                            if self.read_state == 0:
                                header = self.port.read(1)
                                # Header byte
                                header = struct.unpack('B', header)[0]
                                if (header == HEADER):
                                    self.read_state = 1                        
                            elif self.read_state == 1:                        
                                data_raw = self.port.read(2)
                                # Data bytes
                                data_raw = struct.unpack('2B', data_raw)                        
                                self.read_state = 2
                            elif self.read_state == 2:                        
                                tail = self.port.read(1)
                                # Tail byte
                                tail = struct.unpack('B', tail)[0]
                                if (tail == TAIL):
                                    # Handle data only if tail packet is correct
                                    data = self.get_data(data_raw, 16)
                                    #self.signals.prog.emit(1)
                                    self.signals.data.emit(data)
                                    self.read_state = 0                     
                            time.sleep(0.001)
                        
                    except serial.SerialException:
                        self.signals.status.emit(self.port_name, 2)
                        logging.exception("Cannot communicate with port {}. Please check the connection and try again.".format(self.port_name))

            if self.is_killed:
                self.port.close()
                CONN_STATUS = False
                logging.info("Port {} closed.".format(self.port_name))                
                logging.info("Killing the process")
                return

    @pyqtSlot()
    def send(self, char):
        """!
        @brief Function to send a single char on serial port.
        """
        try:
            self.port.write(char.encode('utf-8'))
            logging.info("Written {} on port {}.".format(char, self.port_name))
        except:
            logging.info("Could not write {} on port {}.".format(char, self.port_name))

    @pyqtSlot()
    def send_voltage(self, Positive_potential, Negative_potential):
        """!
        @brief Function to send the initial and final values of the CV.
               Sent positive also the negative one.
        """
        Negative_potential = -Negative_potential

        self.data_buffer = [k for k in range(0, 4)]
        self.data_buffer[0] = H_CV
        self.data_buffer[1] = Negative_potential >> 8
        self.data_buffer[2] = Negative_potential & 0xFF   
        self.data_buffer[3] = T_CV

        self.port.write(self.data_buffer)
        logging.info("Written {} on port {}.".format(self.data_buffer[2], self.port_name))

        self.data_buffer[1] = Positive_potential >> 8
        self.data_buffer[2] = Positive_potential & 0xFF 
        self.port.write(self.data_buffer)
        logging.info("Written {} on port {}.".format(self.data_buffer[2], self.port_name))

    @pyqtSlot()
    def send_crono(self, Chrono_potential):
        """!
        @brief Function to send the voltage value for the Chrono.
               Sent positive also the negative one.
        """
        self.data_crono = [k for k in range(0, 4)]
        if Chrono_potential < 0:
            Chrono_potential = -Chrono_potential
            self.data_crono[0] = H_CN
            self.data_crono[1] = Chrono_potential >> 8
            self.data_crono[2] = Chrono_potential & 0xFF   
            self.data_crono[3] = T_CN
        else:
            self.data_crono[0] = H_C
            self.data_crono[1] = Chrono_potential >> 8
            self.data_crono[2] = Chrono_potential & 0xFF   
            self.data_crono[3] = T_C

        self.port.write(self.data_crono)
        logging.info("Written {} on port {}.".format(self.data_crono[2], self.port_name))


    @pyqtSlot()
    def get_data(self, data_raw, n_bit):
        """!
        @brief Retrieve data from 2 bytes and computes two's complement 
               to reconstruct negative values
        """ 
        data = data_raw[0] << 8 | data_raw[1]
        
        if(data & (1<<(n_bit-1))) !=0:
            data = data - (1<<n_bit)
        return data
        
    
    @pyqtSlot()
    def flush_port(self):        
        """!
        @brief Flush the serial port to avoid reading old data.
        """
        if CONN_STATUS:
            self.port.reset_input_buffer()
            self.port.reset_output_buffer()