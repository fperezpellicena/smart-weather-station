/*
 * weatherData.h
 */
#include <stdint.h>
#ifndef WEATHERDATA_H_
#define WEATHERDATA_H_

#define WEEK_DAYS		7
#define QUARTER_A_DAY	48

typedef struct WeatherStation WeatherStation;

struct WeatherStation{
	uint16_t rainWeek[WEEK_DAYS];
	uint16_t temperature[QUARTER_A_DAY*WEEK_DAYS];
	uint16_t pressure[QUARTER_A_DAY*WEEK_DAYS];
	uint16_t windSpeed[QUARTER_A_DAY*WEEK_DAYS];
	uint16_t windVane[QUARTER_A_DAY*WEEK_DAYS];
};

#endif /* WEATHERDATA_H_ */
