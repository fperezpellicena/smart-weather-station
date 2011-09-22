#ifndef UART_H_
#define UART_H_

#include "stdint.h"

enum Baudrate {
	BD_9K6 = 103,
	BD_57K6 = 16,
	BD_250K = 3
};

/**
 * Inicia la uart, por defecto con 8-N-1 (8 bits de datos, sin paridad y un bit de stop).
 * Controla el registro PRR: Arranca la uart al llamar a este método.
 * La interrupción de recepción completada se habilita, no así la de transmisión.
 *
 * @param baudrate	baudrate por defecto
 */
void uartInit(enum Baudrate baudrate);

/**
 * Quita la alimentación del bloque UART actuando sobre el registro PRR
 */
void uartOff();

/**
 * Envía un único byte de datos por la uart.
 * Comprueba que se ha realizado correctamente el envío.
 *
 * @param data	dato a enviar
 */
void send(uint8_t data);

/**
 * Recibe un byte por la uart
 *
 * @return dato recibido
 */
uint8_t receive();


#endif /* UART_H_ */
