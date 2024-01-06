#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>

// tipologie memorie conosciute
typedef enum {
  AT28C64,
  AT28C256,
  NONE
} e_rom_type;


// invia il comando di richiesta della versione del firmware
int requestFirmware(int fd);

// invia il comando di richiesta lettura della memoria
int requestRead(int fd, e_rom_type romtype);

// invia il comando di richiesta scrittura della memoria
int requestWrite(int fd, e_rom_type romtype);

// legge e visualizza la risposta dal programmatore
int readAnswer(int fd, long msec);

// legge la risposta dal programmatore con il contenuto della memoria e lo salva sul file indicato, attende la risposta per max msec millisecondi 
int readEprom(int fd, e_rom_type romtype, char* filename, long msec);

// invia al prorammatore i dati da scrivere leggendoli dal file indicato, per ogni byte attende al massimo msecforbyte millisecondi
int writeEprom(int fd, e_rom_type romtype, char* filename, long msecforbyte);


// applicazione principale
int main (int argc, char **argv) {
  // selezione memoria di default
  e_rom_type romtype = AT28C64;

  // indicatore se lettura o scritture della memoria
  bool writemem = false;

  // nome del device seriale a cui è collegato il programmatore
  char *device = NULL;

  // nome del file da leggere o scrivere
  char *filename = NULL;

  // effettua il parsing dei parametri passati da linea di comando
  int c;
  while ((c = getopt (argc, argv, "d:f:wt:")) != -1) {
    switch (c) {
      // nome della seriale alla quale è connesso il programmatore
      case 'd':
        device = optarg;
        break;
      // nome del file da leggere o scrivere
      case 'f':
        filename = optarg;
        break;
      // selezione tipologia di memoria
      case 't':
        romtype = atoi(optarg);
        if (romtype < 0 || romtype >= NONE) {
          printf("unknown romtype\n");
          return -1;
        }
        break;
      // opzione per la scrittura della memoria
      case 'w':
        writemem = true;
        break;
    }
  }

  // se non sono stati impostati gli argomenti obbligatori visualizza l'help ed esce
  if (device == NULL || filename == NULL) {
    printf("use: AT28CProgrammer -d <device> -f <filename> [-w] [-t <romtype>]\n");
    printf("\t-d: serial port\n");
    printf("\t-f: file name to read or write\n");
    printf("\t-w: set to write eprom (default is read)\n");
    printf("\t-t: rom type 0 = AT28C64 (default), 1 = AT28C256\n");
    printf("\texample: AT28CProgrammer -d /dev/ttyUSB0 -f /tmp/dump.bin -t 1\n");
    printf("\texample: AT28CProgrammer -d /dev/ttyUSB0 -f /tmp/towrite.bin -t 0 -w\n");
    return -1;
  }

  // visualizza la memoria selezionata
  if (romtype == AT28C64) {
    printf("selected AT28C64\n");
  } else if (romtype == AT28C256) {
    printf("selected AT28C256\n");
  }

  // apre la comunicazione con il programmatore tramite la porta seriale
  int fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    printf("open_port: Unable to open device\n");
    return -1;
  }

  // impostazione dei parametri della porta seriale
  struct termios tio; 
  if (tcgetattr(fd, &tio) != 0) { 
    close(fd);
    printf("error tcgetattr\n"); 
    return -1; 
  } 

  // minimum number of character for non canonical read
  tio.c_cc[VMIN] = 1;
  // timeout in deciseconds for non canonical read
  tio.c_cc[VTIME] = 0;
  tio.c_iflag = 0;
  tio.c_lflag = 0;
  tio.c_cflag = 0;
  tio.c_oflag = 0;

  // ignore break condition on input
  tio.c_iflag |= IGNBRK;
  // 8 bit data
  tio.c_cflag |= CS8;
  // enable receiver, lower modem contron on close device, ignore input modem control lines
  tio.c_cflag |= CREAD | HUPCL | CLOCAL;

  // 115200 baud
  cfsetispeed(&tio, B115200);
  cfsetospeed(&tio, B115200);

  // apply settings to serial port 
  if (tcsetattr(fd, TCSANOW, &tio) != 0) { 
    close(fd);
    printf("error tcsetattr\n"); 
    return -1; 
  } 

  // imposta la seriale in modalità non blocking durante la lettura e pulisce eventuali comunicazioni residue
  fcntl(fd, F_SETFL, FNDELAY);
  usleep(100000);
  tcflush(fd, TCIOFLUSH);

  // invia il comando di richiesta firmware
  if (requestFirmware(fd) == -1) {
    close(fd);
    printf("error request firmware version\n");
    return -1;
  }

  // attende la risposta per un massimo di 100 ms (se il dispositivo non è stato resettato nell'apertura della comunicazione risponderà qui)
  if (readAnswer(fd, 100) == -1) {
    // non ha ricevuto la risposta alla versione firmware  
    // attende l'eventuale intestazione inviata dal programmatore per massimo 1.5 secondi
    if (readAnswer(fd, 1500) == -1) {
      close(fd);
      printf("error reading header\n");
      return -1;
    }

    // invia il comando di richiesta firmware
    if (requestFirmware(fd) == -1) {
      close(fd);
      printf("error request firmware version\n");
      return -1;
    }

    // attende la risposta contentente la versione firmware per un massimo di 100 ms
    if (readAnswer(fd, 100) == -1) {
      close(fd);
      printf("error reading firmware version\n");
      return -1;
    }
  }

  // verifica se richiesta scrittura o lettura della memoria
  if (writemem) {
    // invia il comando di richiesta scrittura della memoria selezionata
    if (requestWrite(fd, romtype) == -1) {
      close(fd);
      printf("error request write eprom\n");
      return -1;
    }
    // invia il contenuto del file da scrivere, per ogny byte scritto attende al massimo 10 ms per la scrittura
    if (writeEprom(fd, romtype, filename, 100) == -1) {
      close(fd);
      printf("error write eprom\n");
      return -1;
    }
  } else {
    // invia il comando di richiesta lettura della memoria selezionata
    if (requestRead(fd, romtype) == -1) {
      close(fd);
      printf("error request read eprom\n");
      return -1;
    }
    // legge la risposta con il contenuto della memoria e lo salva su file
    if (readEprom(fd, romtype, filename, 100) == -1) {
      close(fd);
      printf("error reading eprom\n");
      return -1;
    }
  }

  close(fd);

  fflush(stdout);

  return 0;
}

// invia il comando di richiesta della versione del firmware
int requestFirmware(int fd) {
  const char* cmdGetVersion = "VERSION=?\r";
  return write(fd, cmdGetVersion, strlen(cmdGetVersion));
}

// invia il comando di richiesta lettura della memoria
int requestRead(int fd, e_rom_type romtype) {
  tcflush(fd, TCIOFLUSH);

  const char* cmdReadAT28C64 = "READEEPROM=8192\r";
  const char* cmdReadAT28C256 = "READEEPROM=32768\r";
  if (romtype == AT28C64) {
    return write(fd, cmdReadAT28C64, strlen(cmdReadAT28C64));
  } else if (romtype == AT28C256) {
    return write(fd, cmdReadAT28C256, strlen(cmdReadAT28C256));
  }
  return -1;
}

// invia il comando di richiesta scrittura della memoria
int requestWrite(int fd, e_rom_type romtype) {
  tcflush(fd, TCIOFLUSH);

  const char* cmdWriteAT28C64 = "WRITEEEPROM=8192\r";
  const char* cmdWriteAT28C256 = "WRITEEEPROM=32768\r";
  if (romtype == AT28C64) {
    return write(fd, cmdWriteAT28C64, strlen(cmdWriteAT28C64));
  } else if (romtype == AT28C256) {
    return write(fd, cmdWriteAT28C256, strlen(cmdWriteAT28C256));
  }
  return -1;
}

// legge e visualizza la risposta dal programmatore
int readAnswer(int fd, long msec) {
  int readed = 0;
  while (true) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec = (msec * 1000) / 1000000;
    tv.tv_usec = (msec * 1000) % 1000000;

    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
      printf("error select\n");
      return -1;
    } else if (retval > 0) {
      // ricevuto risposta legge 1 carattere e lo stampa
      char c;
      read(fd, &c, 1);
      readed++;
      // filtra i caratteri di line feed e carriage return
      if (c != '\n' && c != '\r') {
        printf("%c", c);
      }
      // ha ricevuto almeno un carattere,
      // imposta il timeout per i prossimi caratteri a 100 ms
      msec = 100;
    } else {
      // timeout attesa risposta
      break;
    }
  }

  // se ha ricevuto dei caratteri solo alla fine visualizza un fine riga
  if (readed) {
    printf("\n");
    return 0;
  }

  // se non ha ricevuto risposta ritorna una indicazione di errore
  return -1;
}

// legge la risposta dal programmatore con il contenuto della memoria e lo salva sul file indicato, attende la risposta per max msec millisecondi 
int readEprom(int fd, e_rom_type romtype, char* filename, long msec) {
  int writefd = -1;
  int totalbytes = 0;
  int readed = 0;
  int lastperc = 0;
  if (romtype == AT28C64) {
    totalbytes = 8192;
  }
  if (romtype == AT28C256) {
    totalbytes = 32768;
  }
  unlink(filename);
  writefd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (writefd == -1) {
    printf("error opening output file\n");
    return -1;
  }
  while (true) {
    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec = (msec * 1000) / 1000000;
    tv.tv_usec = (msec * 1000) % 1000000;

    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
      printf("error select\n");
      return -1;
    } else if (retval > 0) {
      char c;
      read(fd, &c, 1);
      readed++;
      write(writefd, &c, 1);
      int perc = readed * 100 / totalbytes;
      if (perc != lastperc) {
        printf("<- read percent: %d%%\r", perc);
        fflush(stdout);
        lastperc = perc;
      }
    } else {
      // timeout attesa risposta
      break;
    }
  }

  if (readed) {
    printf("\n");
  }

  // visualizza il numero di bytes ricevuti
  printf("read: %d\n", readed);
  close(writefd);

  // verifica se ha ricevuto il numero di bytes attesi
  if (readed != totalbytes) {
    return -1;
  }

  return 0;
}

// invia al prorammatore i dati da scrivere leggendoli dal file indicato, per ogni byte attende al massimo msecforbyte millisecondi
int writeEprom(int fd, e_rom_type romtype, char* filename, long msecforbyte) {
  int totalbytes = 0;
  int written = 0;
  int lastperc = 0;
  if (romtype == AT28C64) {
    totalbytes = 8192;
  }
  if (romtype == AT28C256) {
    totalbytes = 32768;
  }
  int readfd = open(filename, O_RDONLY);
  if (readfd == -1) {
    printf("error opening input file\n");
    return -1;
  }
  char c;
  while (read(readfd, &c, 1) == 1) {
    write(fd, &c, 1);

    fd_set rfds;
    struct timeval tv;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    tv.tv_sec = (msecforbyte * 1000) / 1000000;
    tv.tv_usec = (msecforbyte * 1000) % 1000000;

    retval = select(fd + 1, &rfds, NULL, NULL, &tv);
    if (retval == -1) {
      printf("error select\n");
      return -1;
    } else if (retval > 0) {
      char c;
      read(fd, &c, 1);
    } else {
      // timeout attesa risposta scrittura byte
      break;
    }

    written++;
    int perc = written * 100 / totalbytes;
    if (perc != lastperc) {
      printf("-> write percent: %d%%\r", perc);
      fflush(stdout);
      lastperc = perc;
    }
  }

  if (written) {
    printf("\n");
  }

  // visualizza il numero di bytes scritti
  printf("written: %d\n", written);
  close(readfd);

  // verifica se ha scritto il numero di bytes attesi
  if (written != totalbytes) {
    return -1;
  }

  return 0;
}
