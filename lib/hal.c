/* hal.c */

#include <avr/io.h>
//#include <util/delay.h>
#include "hal.h"
#include "i2c.h"

// Configura la USART
void usart_init() {
	UCSR0A &= ~_BV(U2X0);//No double speed
	UCSR0A &= ~_BV(MPCM0);//No multiprocessor comm
	UCSR0A &= ~_BV(TXC0);//Transmit complete default 0
	UCSR0B |= _BV(RXCIE0);//RX interrupt enable
	UCSR0B &= ~_BV(TXCIE0);
	UCSR0B &= ~_BV(UDRIE0);
	UCSR0B |= _BV(RXEN0);//Enable RX
	UCSR0B |= _BV(TXEN0);//Enable TX
	UCSR0B &= ~_BV(UCSZ02);
	UCSR0C &= ~_BV(UMSEL00);
	UCSR0C &= ~_BV(UMSEL01);
	UCSR0C &= ~_BV(UPM00);
	UCSR0C &= ~_BV(UPM01);
	UCSR0C &= ~_BV(USBS0);
	UCSR0C |= _BV(UCSZ00);//8 bit data
	UCSR0C |= _BV(UCSZ01);//idem
	UBRR0 = 103;
}

void uart_send(unsigned char byte){
	while((UCSR0A&(1<<UDRE0)) == 0);
	UDR0 = byte;
}

//// Función para escritura en el sensor BMP085
//BMP085_BUS_WR_RETURN_TYPE bmp085_write(unsigned char device_addr,
//		unsigned char register_addr, unsigned char *register_data,
//		unsigned char write_length) {
//	uart_send('5');
//	i2cSendStart();
//	uart_send('6');
//	i2cWaitForComplete();
//	uart_send('7');
//	i2cSendByte(device_addr); // write 0xEE
//	uart_send('8');
//	i2cWaitForComplete();
//	uart_send('9');
//	i2cSendByte(register_addr); // write register address
//	i2cWaitForComplete();
//	unsigned char index;
//	for (index = 0; index < write_length; index++) {
//		i2cSendByte(*register_data++); // write register data for temp
//		i2cWaitForComplete();
//	}
//	i2cSendStop();
//	return 0;
//}
//
//// Función para lectura del sensor BMP085
//BMP085_BUS_RD_RETURN_TYPE bmp085_read(unsigned char device_addr,
//		unsigned char register_addr, unsigned char *register_data,
//		unsigned char read_length) {
//	uart_send('5');
//	i2cSendStart();
//	uart_send('6');
//	i2cWaitForComplete();
//	uart_send('7');
//	i2cSendByte(device_addr); // write 0xEE
//	uart_send('8');
//	i2cWaitForComplete();
//	uart_send('9');
//	i2cSendByte(register_addr); // write register address
//	uart_send('A');
//	i2cWaitForComplete();
//	uart_send('B');
//	i2cSendStart();
//	uart_send('C');
//	i2cSendByte(0xEF); // write 0xEF
//	uart_send('D');
//	i2cWaitForComplete();
//	uart_send('E');
//	i2cReceiveByte(TRUE);
//	uart_send('F');
//	i2cWaitForComplete();
//	uart_send('G');
//	unsigned int index;
//	for (index = 0; index < read_length - 1; index++) {
//		register_data[index] = i2cGetReceivedByte(); // Get MSB result
//		i2cWaitForComplete();
//	}
//	// Last byte
//	i2cReceiveByte(FALSE);
//	i2cWaitForComplete();
//	register_data[index] = i2cGetReceivedByte(); // Get LSB result
//	i2cWaitForComplete();
//
//	i2cSendStop();
//	uart_send('W');
//	return 0;
//}
//
//// Función de delay
//BMP085_MDELAY_RETURN_TYPE bmp085_delay_msec(unsigned int delay) {
//	_delay_ms(delay);
//}
