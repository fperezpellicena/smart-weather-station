/*
 * Estación meteorológica basada en Arduino
 */

//#include "util/delay.h"
#include "lib/hal.h"
#include "lib/bmp085.h"
#include "lib/weatherData.h"
#include "lib/rprintf.h"
#include "lib/i2c.h"
#include "lib/uart.h"


int main(void) {
	return 0;
}

/*int main() {
	// Inicia la uart
	usart_init();
	rprintfInit(&uart_send);

	// Inicia I2C
	i2cInit();

	// Instancia del sensor
	bmp085_t bmp085;

	// Funciones de lectura/escritura/delay
	bmp085.bus_read = &bmp085_read; //	BMP085_BUS_RD_RETURN_TYPE (*bus_read)( BMP085_BUS_RD_PARAM_TYPES );
	bmp085.bus_write = &bmp085_write; //	BMP085_BUS_WR_RETURN_TYPE (*bus_write)( BMP085_BUS_WR_PARAM_TYPES );
	bmp085.delay_msec = &bmp085_delay_msec; //  BMP085_MDELAY_RETURN_TYPE (*delay_msec)( BMP085_MDELAY_DATA_TYPE );


	// Inicia el sensor
	bmp085_init(&bmp085);

	while (1) {

		rprintf("Temperature in 0.1 deg C: ");
		rprintfNum(10, 16,  TRUE, ' ',   bmp085_get_temperature(bmp085_get_ut()));
		rprintfCRLF();
		rprintf("Pressure: " );
		rprintfNum(10, 16,  TRUE, ' ',   bmp085_get_pressure(bmp085_get_up()));
		rprintfCRLF();


		// For fun, lets convert to altitude
		temp = (double) pressure/101325;
		temp = 1-pow(temp, 0.19029);
		altitude = round(44330*temp);
		printf("Altitude: %ldm\n\n", altitude);

		_delay_ms(5000);
	}
	return 0;
}*/

/**
 * #define QUARTER_A_DAY	96
 #define WEEK_DAYS		6

 WeatherStation weatherStation;
 u08 quarters;
 u08 days;

 u16 rain;


 ISR(RAIN){
 cli();
 rain++;
 sei();
 }

 ISR(TIMER){
 cli();
 // Alimentar ADC
 powerOnAdc();
 // Alimentar UART
 powerOnUart();

 // Iniciar ADC
 adcInit();
 // Configurar uart
 usart_init();

 quarter++;
 if(quarters == QUARTER_A_DAY){
 rain = 0;
 }

 if(days == WEEK_DAYS) {
 days = 0;
 } else {
 days++;
 }

 // Adc off
 powerOffAdc();
 // Uart off
 powerOffUart();
 sei();
 }

 int main() {
 // Alimentar Timer 1
 powerOnTimer1();
 // Configurar timer 1
 timer_init();

 while(1){
 // Dormir
 sleep();
 }
 return 1;
 }
 */

