#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
/* Definimos un tipo de datos para el prescaler y otro para indicar el timer que se utiliza*/
enum Prescaler {
	NO_PRESCALER = 1,
	PRESCALER_X8 = 2,
	PRESCALER_X64 = 3,
	PRESCALER_X256 = 4,
	PRESCALER_X1024 = 5
};

enum Timer {
	TIMER0 = 0,
	TIMER1,
	TIMER2
};

/* Funcion que arranca el timer que le indiques con el prescaler escogido
   y en cada overflow se activa la interrupción de overflow */
void timerOVFInit(enum Timer timer, enum Prescaler prescaler);


/* Funcion que arranca el timer que le indiques con el prescaler escogido
   y compara el valor que lleva contando con el que le pasas. Cuando son
   igual se activa la interrupción del comparador */
void timerOCRAInit(enum Timer timer, enum Prescaler prescaler, uint16_t compareValue);

/* Apaga el timer que indiques */
void timerOff(enum Timer timer);

//uint16_t millisToBinary(uint16_t millis);

#endif /* TIMER_H_ */
