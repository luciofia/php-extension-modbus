/*
 * Arquivo   : modbus.c
 * Descricao : Protocolo Modbus
 * Autor     : Diego
 * Data      : 05/04/2005
 *
 */

#ifndef MODBUS_H
#define MODBUS_H

/* Error codes */
#define TIMEOUT_ERROR -100
#define CRC_ERROR -101
#define DEVICE_ERROR -102
#define READ_HR_ERROR -103
#define READ_IR_ERROR -104
#define WRITE_REG_ERROR -105


/* Buffer size */
#define MBUF_SIZE 256

int modbus_read_holding_registers(int port, unsigned int d, unsigned int a, unsigned int len, unsigned int * datar);
int modbus_read_input_registers(int port, unsigned int d, unsigned int a, unsigned int len, unsigned int * datar);
int modbus_write(int port, unsigned int d, unsigned int a, unsigned int len, unsigned int * dataw);
unsigned int crc_update(const char *s, unsigned int l);

#endif /* MODBUS_H */

