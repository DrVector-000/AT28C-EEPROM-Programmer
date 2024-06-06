import serial
import time

class SerialDevice:

    # Private attributes
    __serial = serial.Serial()

    # Constructor
    def __init__(self, baudrate):
        self.__serial.baudrate = baudrate

    # Private methods
    def __sendcommand(self, command):
        res = ''
        if self.__serial.is_open:
            # flush input buffer
            self.__serial.reset_input_buffer()
            self.__serial.write(command)
            self.__serial.flush()
            
            s = self.__serial.readline()
            print(s)

            split = s.split(b'=')
            res = split[1]
            res = res.replace(b"\r\n", b"").decode('utf-8')

        else:
            print('Serial not open')

        return res

    # Public methods
    def open(self):
        self.__serial.open()
        print('Open ' + repr(self.__serial.port) + ',' + repr(self.__serial.baudrate))

    def close(self):
        self.__serial.close()
        print('Close ' + repr(self.__serial.port))

    def setport(self, port):
        self.__serial.port = port

    def getfwversion(self):
        res = self.__sendcommand(b"VERSION=?\r")
        return res
    
    # Lettura EEPROM
    def readEEPROM(self, size):
        datas = bytearray()

        command = bytes("READEEPROM=" + str(size) + "\r", "utf-8")
        if self.__serial.is_open:
            # flush input buffer
            self.__serial.reset_input_buffer()
            self.__serial.write(command)
            self.__serial.flush()
        
        while (len(datas) < size):
            buf = self.__serial.read()
            datas.extend(buf)

        return datas

    # Scrittura EEPROM
    def writeEEPROM(self, size, pagesize, datas):
        if pagesize != 0:
            command = bytes("WRITEEEPROM=" + str(size) + "," + str(pagesize) + "\r", "utf-8")
        else:
            pagesize = 1
            command = bytes("WRITEEEPROM=" + str(size) + "\r", "utf-8")

        if self.__serial.is_open:
            # flush input buffer
            self.__serial.reset_input_buffer()
            self.__serial.write(command)
            self.__serial.flush()
        
        i = 0
        while (size > i):
            p = 0
            while (p < pagesize):
                index = i + p
                byte = datas[index:index +1]
                self.__serial.write(byte)
                p = p + 1
            
            # Verifica scritttura
            expiredtime = time.perf_counter_ns() + 100000000
            #count = 0
            while (time.perf_counter_ns() < expiredtime):
                count = self.__serial.in_waiting
                if count >= pagesize:
                    buf = self.__serial.read()
                    p = 0
                    while (len(buf) < pagesize):
                        if buf[p] != datas[i + p]:
                            return -1
                        p = p + 1
                    break

            #if time.perf_counter_ns() > expiredtime:
                #return -2

            i = i + pagesize

        return 0
