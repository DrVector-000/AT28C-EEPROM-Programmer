from tkinter import *
from tkinter import ttk
from tkinter import scrolledtext
from datetime import datetime
import SerialHelper
import SerialDevice
import time
from tkinter import filedialog

class TkWindowMain:

    # Private attributes
    serialdevice: SerialDevice
    portscb: ttk.Combobox
    eepromscb: ttk.Combobox
    text_area: scrolledtext.ScrolledText
    btndisconnect: ttk.Button
    btnconnect: ttk.Button
    btnreadram: ttk.Button
    btnreadrom: ttk.Button
    btnwriteram: ttk.Button
    btninfo: ttk.Button


    # Costruttore
    def __init__(self):
        # Istanza oggetto SerialDeviceBase
        self.serialdevice = SerialDevice.SerialDevice(115200)


    # Metodi (accesso privato)
    def __SetControlsState(self, connect):
        if connect:
            self.btndisconnect['state'] = NORMAL
            self.btnconnect['state'] = DISABLED
            self.btnreadram['state'] = NORMAL
            self.btnreadrom['state'] = NORMAL
        else:
            self.btndisconnect['state'] = DISABLED
            self.btnconnect['state'] = NORMAL
            self.btnreadram['state'] = DISABLED
            self.btnreadrom['state'] = DISABLED


    # Funzioni eventi bottoni
    # Evento pressione bottone connetti
    def __Connect(self):
        port = self.portscb.get()
        self.serialdevice.setport(port)
        self.serialdevice.open()
        time.sleep(2)
        
        # Cancella stringhe di log
        self.text_area.delete(1.0, END)

        # Lettura versione del firmware
        self.text_area.insert(END, 'AT28C Programmer\n')
        ver = self.serialdevice.getfwversion()
        self.text_area.insert(END, 'Firmware version: ' + ver + '\n\n')

        self.__SetControlsState(True)

    # Eventro pressione bottone disconnessione
    def __Disconnect(self):
        self.serialdevice.close()
        self.__SetControlsState(False)

    # Evento pressione bottone Read EEPROM
    def __ReadEEPROM(self):
        size = 0
        
        match self.eepromscb.get():
            case "AT28C64":
                size = 8192
            case "AT28C64B":
                size = 8192
            case "AT28C256":
                size = 32768
        
        if size > 0:
            now = datetime.now()
            self.text_area.insert(END, now.strftime("%H:%M:%S") + ' Start reading EEPROM\n')

            datas = self.serialdevice.readEEPROM(size)

            now = datetime.now()
            self.text_area.insert(END, now.strftime("%H:%M:%S") + ' End reading EEPROM\n')
            i = len(datas)

            if i > 0:
                f =  filedialog.asksaveasfile(mode='wb', defaultextension=".bin")
                if f is None:
                    return
                f.write(datas)
                f.close()

    # Evento pressione bottone Write EEPROM
    def __WriteEEPROM(self):
        pagesize = 0
        eepromtype = self.eepromscb.get()

        if eepromtype == "":
            self.text_area.insert(END, 'Select the type of EEPROM\n')
            return
        
        if eepromtype == "AT28C256" or eepromtype == "AT28C64B": 
            pagesize = 64
        
        f = filedialog.askopenfile(mode="rb", defaultextension=".bin")
        if f is None:
            return
        
        datas = f.read()
        f.close()
        size = len(datas)

        now = datetime.now()
        self.text_area.insert(END, now.strftime("%H:%M:%S") + ' Start writing EEPROM\n')

        ret = self.serialdevice.writeEEPROM(size, pagesize, datas)

        now = datetime.now()
        self.text_area.insert(END, now.strftime("%H:%M:%S") + ' End writing EEPROM ' + str(ret) + '\n')


    # Metodi (accesso pubblico)
    def Show(self):    
        root = Tk()
        root.title("AT28C Programmer")
        root.eval('tk::PlaceWindow . center')

        # Frame Connessione
        connframe = Frame(root)
        connframe.pack(expand=False, fill=X, anchor=NW)

        ttk.Label(connframe, text="Serial Port").grid(column=1, row=1, sticky=(W, E), padx=5, pady=5)

        # Lista porte seriali disponibili
        selports = StringVar()
        self.portscb = ttk.Combobox(connframe, textvariable=selports)
        self.portscb['values'] = SerialHelper.GetSerialPorts()
        self.portscb['state'] = 'readonly'
        self.portscb.grid(column=2, row=1, sticky=(W, E), padx=5, pady=5)

        # Bottoni connessione
        self.btnconnect = ttk.Button(connframe, text="Connect", command=self.__Connect)
        self.btnconnect.grid(column=3, row=1, sticky=W, padx=5, pady=5)
        self.btnconnect['state'] = NORMAL
        self.btndisconnect = ttk.Button(connframe, text="Disconnect", command=self.__Disconnect)
        self.btndisconnect.grid(column=4, row=1, sticky=W, padx=5, pady=5)
        self.btndisconnect['state'] = DISABLED

        # Finestra logs
        self.textframe = Frame(root, background="red")
        self.textframe.pack(expand=True, fill=BOTH, anchor=NW)

        self.text_area = scrolledtext.ScrolledText(self.textframe, wrap=WORD, width=50, height=10, font=("Times New Roman", 12))
        self.text_area.pack(fill=BOTH, expand=True, anchor=CENTER)       

        # Frame tools
        toolsframe = Frame(root)
        toolsframe.pack(expand=False, fill=X, anchor=SW)

        ttk.Label(toolsframe, text="EEPROM Type").grid(column=1, row=1, sticky=(W, E), padx=5, pady=5)

        seleeproms = StringVar()
        self.eepromscb = ttk.Combobox(toolsframe, textvariable=seleeproms)
        self.eepromscb['values'] = ('AT28C64', 'AT28C64B', 'AT28C256')
        self.eepromscb['state'] = 'readonly'
        self.eepromscb.grid(column=2, row=1, sticky=(W, E), padx=5, pady=5)


        self.btnreadrom = ttk.Button(toolsframe, text="Read EEPROM", command=self.__ReadEEPROM)
        self.btnreadrom.grid(column=1, row=2, sticky=W, padx=5, pady=5)
        self.btnreadrom['state'] = DISABLED

        self.btnreadram = ttk.Button(toolsframe, text="Write EEPROM", command=self.__WriteEEPROM)
        self.btnreadram.grid(column=2, row=2, sticky=W, padx=5, pady=5)
        self.btnreadram['state'] = DISABLED

        root.mainloop()
