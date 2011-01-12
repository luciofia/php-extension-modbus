#ifndef SERIAL_H
#define SERIAL_H

int serial_open(char* port, int baudrate, int bits, char parity, int stopbits, int timeout);
int serial_getch(int port);
int serial_putch(int port, char c);


#endif /* SERIAL_H */

