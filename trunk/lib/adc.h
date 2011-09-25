/** 
La librería está bien, pero no sabemos por qué sólo funciona correctamente
con los prescaler X64 y X128
**/

#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

enum AdcChannel {
	ADC_CH_0 = 0,
	ADC_CH_1 = 1,
	ADC_CH_2 = 2,
	ADC_CH_3 = 3,
	ADC_CH_4 = 4,
	ADC_CH_5 = 5,

};

enum AdcPrescaler {
	ADC_PRESCALER_X2 = 1,
	ADC_PRESCALER_X4 = 2,
	ADC_PRESCALER_X8 = 3,
	ADC_PRESCALER_X16 = 4,
	ADC_PRESCALER_X32 = 5,
	ADC_PRESCALER_X64 = 6,		/** Usar los prescaler de X64 o X128 **/
	ADC_PRESCALER_X128 = 7
};

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
void adcInit(enum AdcPrescaler prescaler);

/**
 * Configura el canal(MUX).
 * Deshabilita la opción digital (DIDR0)
 * Inicia una conversión (ADSC=1)
 */
void adcStartConversion(enum AdcChannel channel);

/** Devuelve el contenido de ADCH y ADCL */
uint16_t getAdcConversion();

/** Quita la alimentación (PRR) */
void adcOff();


#endif /* ADC_H_ */
