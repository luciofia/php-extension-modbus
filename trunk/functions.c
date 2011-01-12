/*
 * Arquivo   : modbus.c
 * Descricao : Protocolo Modbus
 * Autor     : Diego
 * Data      : 05/04/2005
 *
 */

#include <stdio.h>
#include "serial.h"
#include "modbus.h"

/*
 * modbus_read_holding_registers
 * Read holding registers
 * Parameters:
 * port - serial port file descriptor
 * d - device address
 * a - register address
 * len - number of registers to read
 * *datar - array to store read data
 *
 */
int modbus_read_holding_registers(int port, unsigned int d, unsigned int a, unsigned int len, unsigned int * datar)
{
  unsigned int crc, crcr;
  int i, n;
  int c;
  unsigned char mbuf[MBUF_SIZE];
  int response_len;

  i = 0;

  serial_flush(port);

  mbuf[i++] = d;
  mbuf[i++] = 3;
  mbuf[i++] = (unsigned char) (a >> 8);
  mbuf[i++] = (unsigned char) a;
  mbuf[i++] = (unsigned char) (len >> 8);
  mbuf[i++] = (unsigned char) len;
  crc = crc_update (mbuf, 6);
  mbuf[i++] = (unsigned char) crc;
  mbuf[i++] = (unsigned char) (crc >> 8);

#if VERBOSE
  printf("sending read holding register command.\n");
#endif /* VERBOSE */

  for (n = 0; n < i; n++)
    {
      serial_putch(port, mbuf[n]);
      #if VERBOSE
      printf("send [%02X]\n", mbuf[n]);
      #endif /* VERBOSE */
    }


  response_len = 5 + (len * 2);
  #if VERBOSE
  printf("waiting response...\nresponse length: %d bytes\n", response_len);
  #endif /* VERBOSE */

  for (i=0; i<response_len; i++)
  {
    c = serial_getch(port);

    if (c < 0)
    {
#if VERBOSE
      printf("timeout!\n");
#endif /* VERBOSE */
      return (TIMEOUT_ERROR);
    }

#if VERBOSE
    printf("received [%02X]\n", c);
#endif /* VERBOSE */

    if (i == 1)
    {
      if (c == 0x83) /* Error */
      {
        response_len = 5;
      }
    }
    mbuf[i] = c;
  }

  /* Destination */
  if (mbuf[0] != d)
  {
    return (DEVICE_ERROR);
  }

  /* OP Code (0x03) */
  if (mbuf[1] != 3)
    {
      return READ_HR_ERROR;
    }

  /* Data */
  for (i=0; i<(len * 2); i+=2)
  {
    *datar = (mbuf[3+i] << 8) + mbuf[4+i];
    datar += 1;
  }

  /* CRC */
  crcr = (mbuf[4+(len*2)] << 8) + mbuf[3+(len*2)];
  crc = crc_update(mbuf, 3+(len * 2));

  if(crcr != crc)
  {
    return(CRC_ERROR);
  }
  else
  {
    return len;
  }
}

/*
 * modbus_read_input_register
 * Read input registers
 * Parameters:
 * port - serial port file descriptor
 * d - device address
 * a - register address
 * len - number of registers to read
 * *datar - array to store read data
 *
 */
int modbus_read_input_registers(int port, unsigned int d, unsigned int a, unsigned int len, unsigned int * datar)
{
  unsigned int crc, crcr;
  int i, n;
  int c;
  unsigned char mbuf[MBUF_SIZE];
  int response_len;

  serial_flush(port);

  i = 0;

  mbuf[i++] = d;
  mbuf[i++] = 4; /* Read input register */
  mbuf[i++] = (unsigned char) (a >> 8);
  mbuf[i++] = (unsigned char) a;
  mbuf[i++] = (unsigned char) (len >> 8);
  mbuf[i++] = (unsigned char) len;
  crc = crc_update (mbuf, 6);
  mbuf[i++] = (unsigned char) crc;
  mbuf[i++] = (unsigned char) (crc >> 8);

  for (n = 0; n < i; n++)
  {
    serial_putch(port, mbuf[n]);
  }

  response_len = 5 + (2 * len);

  for(i=0; i<response_len; i++)
  {
    c = serial_getch(port);

    if (c < 0)
    {
      return (TIMEOUT_ERROR);
    }
    
    if (i == 1)
    {
      if (c == 0x84)
      {
        return (READ_IR_ERROR);
      }
    }
    mbuf[i] = c;
  }

  /* Destination */
  if(mbuf[0] != d)
  {
    return (DEVICE_ERROR);
  }

  /* OP Code (0x04) */
  if (mbuf[1] != 4)
  {
    return (READ_IR_ERROR);
  }

  /* Data */
  for (i=0; i<(len * 2); i+=2)
  {
    *datar = (mbuf[3+i] << 8) + mbuf[4+i];
    datar += 1;
  }

  /* CRC */
  crcr = (mbuf[4+(len*2)] << 8) + mbuf[3+(len*2)];
  crc = crc_update(mbuf, 3+(len*2));

  if(crc != crcr)
  {
    return(CRC_ERROR);
  }
  else
  {
    return len;
  }
}


/*
 * modbus_write
 * Envia comando de escrita do protocolo modbus no endereco modbus d, endereco de memoria a, l bytes, escreve *b pela serial port
 */
int modbus_write(int port, unsigned int d, unsigned int a, unsigned int len, unsigned int * dataw)
{
  unsigned int crc, crcr;
  int i, n;
  int c;
  unsigned char mbuf[MBUF_SIZE];
  int response_len;

  serial_flush(port);

  n = 0;

  mbuf[n++] = d;
  mbuf[n++] = 0x10;
  mbuf[n++] = (unsigned char) (a >> 8);
  mbuf[n++] = (unsigned char) a;
  mbuf[n++] = (unsigned char) 0;
  mbuf[n++] = (unsigned char) 1;
  mbuf[n++] = (unsigned char) 2;

  for (i=0; i<len; i++)
  {
    *(unsigned int *)&mbuf[n] = *dataw;
    n += 2;
  }

  crc = crc_update(&mbuf[0], n);
  mbuf[n++] = (unsigned char) crc;
  mbuf[n++] = (unsigned char) (crc >> 8);

  for (i = 0; i < n; i++)
  {
    serial_putch(port, mbuf[i]);
  }

  response_len = 8;

  for (i=0; i<response_len; i++)
  {
    c = serial_getch(port);

    if (c < 0)
    {
      return (TIMEOUT_ERROR);
    }

    if (i == 1)
    {
      if (c == 0x90)
      {
        response_len = 5;
      }
    }
    mbuf[i] = c;
  }

  /* Destination */
  c = serial_getch(port);
  if (mbuf[0] != d)
  {
    return (DEVICE_ERROR);
  }

  /* OP Code (0x10) */
  if (mbuf[1] != 0x10)
  {
    return (WRITE_REG_ERROR);
  }

  /* CRC */
  crcr = *(unsigned int *)&mbuf[6];

  crc = crc_update(mbuf, 6);

  if(crc == crcr)
  {
    return(len);
  }
  else
  {
    return(CRC_ERROR);
  }
}

/*
 * crc_update
 * calcula o crc do buffer *s para l bytes
 */
unsigned int crc_update(const char *s, unsigned int l)
{
  /* bit by bit algorithm with augmented zero bytes. */
  /* does not use lookup table, suited for polynom orders between 1...32. */

  unsigned int i, j, c, b;
  unsigned int crc = 0xffff;

  for (i = 0; i < l; i++)
  {
    c = (unsigned int)*s++;

    for (j = 0x01; j < 0x100; j <<= 1)
    {
      b = crc & 0x0001;
      crc >>= 1;
      if (c & j)
        b ^= 0x0001;
      if (b)
        crc ^= 0xa001;
    }
  }
  return (crc);
}

