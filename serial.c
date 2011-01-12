#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


#include "serial.h"

/* serial_open
 * open serial port
 * return file descriptor
 *
 */
int serial_open(char* port, int baudrate, int bits, char parity, int stopbits, int timeout)
{
  int fd;
  struct termios options;

  fd = open(port, O_RDWR | O_NOCTTY);

  if (fd == -1)
    {
      return -1;
    }

  /* Get port options */
  tcgetattr(fd, &options);

  /* Set baudrate */
  options.c_cflag &= ~CBAUD;
  switch(baudrate)
    {
      case 1200:
        options.c_cflag |= B1200;
        break;
      case 2400:
        options.c_cflag |= B2400;
        break;
      case 4800:
        options.c_cflag |= B4800;
        break;
      case 9600:
        options.c_cflag |= B9600;
        break;
      case 19200:
        options.c_cflag |= B19200;
        break;
      case 38400:
        options.c_cflag |= B38400;
        break;
      case 57600:
        options.c_cflag |= B57600;
          break;
      case 115200:
        options.c_cflag |= B115200;
        break;
      default:
        options.c_cflag |= B9600;
        break;
    }

  /* Set bits size */
  options.c_cflag &= ~CSIZE;
  if (bits == 7)
    {
      options.c_cflag |= CS7;
    }
  else
    {
      options.c_cflag |= CS8;
    }

  /* Set parity */
  switch(parity)
    {
      case 'E':
      case 'e':
        options.c_cflag |= PARENB;
        options.c_cflag &= ~PARODD;
        break;
      case 'O':
      case 'o':
        options.c_cflag |= PARENB;
        options.c_cflag |= PARODD;
        break;
      case 'N':
      case 'n':
      default:
        options.c_cflag &= ~PARENB;
	options.c_iflag |= IGNPAR;
        break;
    }
  options.c_iflag |= ICRNL;

  /* Set stop bits */
  if (stopbits == 2)
    {
      options.c_cflag |= CSTOPB;
    }
  else
    {
      options.c_cflag &= ~CSTOPB;
    }

  /* Enable receiver and set local mode */
  options.c_cflag |= (CLOCAL | CREAD);

  options.c_cc[VMIN] = 0;
  options.c_cc[VTIME] = timeout;

  options.c_oflag = 0;
  options.c_lflag = 0;


  /* Clean port */
  tcflush(fd, TCIFLUSH);

  fcntl(fd, F_SETFL, 0);

  /* Set options on port */
  tcsetattr(fd, TCSANOW, &options);

  /* Clean I/O and device */
  tcflush(fd, TCIOFLUSH);
    
  return fd;
}

void serial_flush(int fd)
{
  /* Clean I/O and device */
  tcflush(fd, TCIOFLUSH);
}

int serial_putch(int port, char c)
{
  return write(port, &c, 1);
}

int serial_getch(int port)
{
  int len;
  unsigned char r;
  int c;

  len = read(port, &r, 1);
  if (len > 0)
    {
      c = r;
      return c;
    }
  else
    {
      return -1;
    }
}

