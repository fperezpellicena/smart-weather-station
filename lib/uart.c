#include "uart.h"
#include <avr/io.h>
#include <avr/power.h>

/**
 * Inicia la uart, por defecto con 8-N-1 (8 bits de datos, sin paridad y un bit de stop).
 * Controla el registro PRR: Arranca la uart al llamar a este método.
 * La interrupción de recepción completada se habilita, no así la de transmisión.
 *
 * @param baudrate	baudrate por defecto
 */
void uartInit(enum Baudrate baudrate) {
	/* Habilita la interrupción de dato recibido */
	UCSR0B |= _BV(RXCIE0);
	/* Habilita la recepción y  la transmisión */
	UCSR0B |= _BV(RXEN0);
	UCSR0B |= _BV(TXEN0);
	/* Modo de transmisión asíncrono */
	UCSR0C &= ~_BV(UMSEL00);
	UCSR0C &= ~_BV(UMSEL01);
	/* Sin paridad */
	UCSR0C &= ~_BV(UPM00);
	UCSR0C &= ~_BV(UPM01);
	/* 1 bit de stop*/
	UCSR0C &= ~_BV(USBS0);
	/* 8 bits de datos */
	UCSR0C |= _BV(UCSZ00);
	UCSR0C |= _BV(UCSZ01);
	UCSR0C &= ~_BV(UCSZ02);
	/* Baudrate */
	UBRR0 = baudrate;
	/* Activa la uart */
	PRR &= ~_BV(PRUSART0);
}

/**
 * Quita la alimentación del bloque UART actuando sobre el registro PRR
 */
void uartOff() {
	PRR |= _BV(PRUSART0);
}

/**
 * Envía un único byte de datos por la uart.
 * Comprueba que se ha realizado correctamente el envío.
 *
 * @param data	dato a enviar
 */
void send(uint8_t data) {
	while(!UDRE0);
	UDR0 = data;
}

/**
 * Recibe un byte por la uart
 *
 * @return dato recibido
 */
uint8_t receive() {
	return UDR0;
}


