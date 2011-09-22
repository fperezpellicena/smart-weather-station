/**
 * Inicia y configura el adc.
 * Establece el prescaler para el reloj del ADC.
 * Configura la referencia AVcc.
 * Configura la interrupción de conversión completada (ADIE=1)
 * Configura el modo de conversión única
 * Configura el modo de ajuste a la derecha (ADLAR=0);
 * Habilita el conversor (ADEN=1)
 * Habilita la alimentación (PRR)
 */

#include"adc.h"
#include <avr/io.h>

void adcInit(enum AdcPrescaler prescaler){
	PRR &= ~_BV(PRADC);  //Activa la alimentación del ADC
	ADCSRA = prescaler;  //Establece el prescaler
	ADCSRA |= _BV(ADEN); //Activa el conversor
	ADMUX &= ~_BV(REFS1); //Configura la referencia AVcc
	ADMUX |= _BV(REFS0);
	ADCSRB &= ~_BV(ADTS0);	 //Modo de conversión única (Free running mode)
	ADCSRB &= ~_BV(ADTS1);
	ADCSRB &= ~_BV(ADTS2);
	ADCSRA |= _BV(ADIE); //Poner a 1 para activar la interrupción de conversión
	ADMUX |= _BV(ADLAR);  //Modo de ajuste a la derecha (resolución 10 bits)
}

/**
 * Configura el canal(MUX).
 * Deshabilita la opción digital (DIDR0)
 * Inicia una conversión (ADSC=1)
 */
void adcStartConversion(enum AdcChannel channel){
	switch (channel){
	case ADC_CH_0:
		ADMUX &= ~_BV(MUX0);
		ADMUX &= ~_BV(MUX1);
		ADMUX &= ~_BV(MUX2);
		ADMUX &= ~_BV(MUX3);
		DIDR0 |= _BV(ADC0D);
		ADCSRA |= _BV(ADSC);
	case ADC_CH_1:
		ADMUX |= _BV(MUX0);
		ADMUX &= ~_BV(MUX1);
		ADMUX &= ~_BV(MUX2);
		ADMUX &= ~_BV(MUX3);
		DIDR0 |= _BV(ADC1D);
		ADCSRA |= _BV(ADSC);
	case ADC_CH_2:
		ADMUX &= ~_BV(MUX0);
		ADMUX |= _BV(MUX1);
		ADMUX &= ~_BV(MUX2);
		ADMUX &= ~_BV(MUX3);
		DIDR0 |= _BV(ADC2D);
		ADCSRA |= _BV(ADSC);
	case ADC_CH_3:
			ADMUX |= _BV(MUX0);
			ADMUX |= _BV(MUX1);
			ADMUX &= ~_BV(MUX2);
			ADMUX &= ~_BV(MUX3);
			DIDR0 |= _BV(ADC3D);
			ADCSRA |= _BV(ADSC);
	case ADC_CH_4:
		ADMUX &= ~_BV(MUX0);
		ADMUX &= ~_BV(MUX1);
		ADMUX |= _BV(MUX2);
		ADMUX &= ~_BV(MUX3);
		DIDR0 |= _BV(ADC4D);
		ADCSRA |= _BV(ADSC);
	case ADC_CH_5:
		ADMUX |= _BV(MUX0);
		ADMUX &= ~_BV(MUX1);
		ADMUX |= _BV(MUX2);
		ADMUX &= ~_BV(MUX3);
		DIDR0 |= _BV(ADC5D);
		ADCSRA |= _BV(ADSC);
	}
}

/** Devuelve el contenido de ADCH y ADCL */
uint16_t getAdcConversion(){

uint16_t adc_value;

adc_value = ADCL;  
adc_value += (ADCH<<8);

return adc_value;

}

/** Quita la alimentación (PRR) */
void adcOff(){
	ADCSRA &= ~_BV(ADEN);	//Hay que deshabilitar la ADC antes de apagarla
	PRR |= _BV(PRADC);		//Apaga el ADC
}
