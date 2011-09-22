#ifndef HAL_H_
#define HAL_H_

#include "bmp085.h"

// Configura la USART
void usart_init();

// Envía un byte
void uart_send(unsigned char byte);

//// Función para escritura en el sensor BMP085
//BMP085_BUS_WR_RETURN_TYPE bmp085_write(unsigned char device_addr,
//		unsigned char register_addr, unsigned char *register_data,
//		unsigned char write_length);
//
//// Función para lectura del sensor BMP085
//BMP085_BUS_RD_RETURN_TYPE bmp085_read(unsigned char device_addr,
//		unsigned char register_addr, unsigned char *register_data,
//		unsigned char read_length);
//
//// Función de delay
//BMP085_MDELAY_RETURN_TYPE bmp085_delay_msec(unsigned int delay);


#endif /* HAL_H_ */
