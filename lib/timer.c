#include "timer.h"
#include <avr/power.h>


void timerOVFInit(enum Timer timer, enum Prescaler prescaler) {
	if(timer == TIMER0) {
			/* Para el timer y resetea el registro contador*/
			TCCR0B = 0;
			TCNT0 = 0;
			/* Habilitar la interrupción de overflow */
			TIMSK0 |= _BV(TOIE0);
			/* Activa el timer */
			PRR &= ~_BV(PRTIM0);
			/* Prescaler */
			TCCR0B = prescaler;
		}

	else if(timer == TIMER1) {
		/* Para el timer y resetea el registro contador*/
		TCCR1B = 0;
		TCNT1 = 0;
		/* Habilitar la interrupción de overflow */
		TIMSK1 |= _BV(TOIE1);
		/* Activa el timer */
		PRR &= ~_BV(PRTIM1);
		/* Prescaler */
		TCCR1B = prescaler;
	}
	else {
			/* Para el timer y resetea el registro contador*/
			TCCR2B = 0;
			TCNT2 = 0;
			/* Habilitar la interrupción de overflow */
			TIMSK2 |= _BV(TOIE2);
			/* Activa el timer */
			PRR &= ~_BV(PRTIM2);
			/* Prescaler */
			TCCR2B = prescaler;
		}
}

void timerOCRAInit(enum Timer timer, enum Prescaler prescaler, uint16_t compareValue) {
	if(timer == TIMER0) {
			/* Para el timer y resetea el registro contador*/
			TCCR0B = 0;
			TCNT0 = 0;
			/* Habilitar la interrupción de comparación con el registro OCR0A */
			TIMSK0 |= _BV(OCIE0A);
			/* Establece el valor de comparación */
			OCR0A = compareValue;
			/* Activa el timer */
			PRR &= ~_BV(PRTIM0);
			/* Prescaler */
			TCCR0B = prescaler;
		}
	else if(timer == TIMER1) {
		/* Para el timer y resetea el registro contador*/
		TCCR1B = 0;
		TCNT1 = 0;
		/* Habilitar la interrupción de comparación con el registro OCR1A */
		TIMSK1 |= _BV(OCIE1A);
		/* Establece el valor de comparación */
		OCR1A = compareValue;
		/* Activa el timer */
		PRR &= ~_BV(PRTIM1);
		/* Prescaler */
		TCCR1B = prescaler;
	}
	else {
		/* Para el timer y resetea el registro contador*/
		TCCR2B = 0;
		TCNT2 = 0;
		/* Habilitar la interrupción de comparación con el registro OCR2A */
		TIMSK2 |= _BV(OCIE2A);
		/* Establece el valor de comparación */
		OCR2A = compareValue;
		/* Activa el timer */
		PRR &= ~_BV(PRTIM2);
		/* Prescaler */
		TCCR2B = prescaler;
	}
}

void timerOff(enum Timer timer) {
	if(timer == TIMER0) {
		PRR |= _BV(PRTIM0);
	}
	else if (timer == TIMER1) {
		PRR |= _BV(PRTIM1);
	}
	else {
			PRR |= _BV(PRTIM2);
		 }
}

