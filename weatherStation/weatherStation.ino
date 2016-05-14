/**
   This file is part of smart_weather_station.

    smart_weather_station is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    smart_weather_station is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with smart_weather_station.  If not, see <http://www.gnu.org/licenses/>.

    _______________________________________________________________________________
    Arduino sketch for Weather device from Sparkfun.
    
    
    Although the inclination for a weather logger is to run it for
    a long time, due to the way Wiring.c implements the millis() function,
    this should be restarted, oh, monthly. The millis() functions overflows
    after about 49 days. We could allow for that here, and handle the
    wraparound, but you've got bigger problems anyway with the delay()
    function at an overflow, so it's best to "reboot".
    
    =========================================================
    ANEMOMETER
    =========================================================
    This is connected to Arduino ground on one side, and pin 2 (for the
    attachInterrupt(0, ...) on the other.
    Pin 2 is pulled up, and the reed switch on the anemometer will send
    that to ground once per revolution, which will trigger the interrupt.
    We count the number of revolutions in 5 seconds, and divide by 5.
    One Hz (rev/sec) = 1.492 mph.
    
    =========================================================
    WIND DIRECTION VANE
    =========================================================
    We use a classic voltage divider to measure the resistance in
    the weather vane, which varies by direction.
    Using a 10K resistor, our ADC reading will be:
       1023 * (R/(10000+R))
    where R is the unknown resistance from the vane. We'll scale
    the 1023 down to a 255 range, to match the datasheet docs.
    
                      +5V
                       |
                       <
                       >     10K
                       <   Resistor
                       <
                       >
                       |
     Analog Pin 5------|
                       |
                       -----------| To weather vane
                                  | (mystery resistance)
                       -----------|
                       |
                       |
                     -----
                      ---
                       -
    The ADC values we get for each direction (based on a 255 max)
    follow, assuming that pointing away from the assembly center
    is sector zero. The sector number is just which 45-degree sector
    it is, clockwise from the "away" direction. The direction
    shown is assuming that "away" is West. Depending how
    you orient the system, you'll have to adjust the directions.
    
    Sector   Reading  Direction
      0        18        W
      1        33        NW
      2        57        N
      7        97        SW
      3       139        NE
      6       183        S
      5       208        SE
      4       232        E
    The values in the ADC table below list the midpoints between
    these, so our reading can vary a bit. We'll pick the first value
    that's >= our reading.
    =========================================================
    RAIN GAUGE
    =========================================================
    Not implemented here. Hey. I live in Seattle. It's ALWAYS raining. Who
    cares how much?
    Okay, it would probably be done the same way as the anemometer, and use
    attachInterrupt(1, ...) on pin 3. Each interrupt represents
    .011 inches of rain, according to the docs.

*/
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define uint  unsigned int
#define ulong unsigned long


/*..............................................................................*/
/* STATION */
#define PIN_ANEMOMETER  2     // Digital 2
#define PIN_VANE        5     // Analog 5

// How often we want to get measures
#define MSECS_CYCLE 5000

volatile int numRevsAnemometer = 0; // Incremented in the interrupt
ulong nextCycle;
ulong time;                         // Millis() at each start of loop().
ulong numBuckettip;

// ADC readings:
#define NUMDIRS 8
ulong   adc[NUMDIRS] = {26, 45, 77, 118, 161, 196, 220, 256};

// These directions match 1-for-1 with the values in adc, but
// will have to be adjusted as noted above. Modify 'dirOffset'
// to which direction is 'away' (it's West here).
char *strVals[NUMDIRS] = {"W","NW","N","SW","NE","S","SE","E"};
byte dirOffset=0;

struct WeatherData {
	String windSpeed;
	String windDir;
	String rain;
	String temperature;
	String pressure;
};

WeatherData weatherData;

/* END STATION */
/*..............................................................................*/

/*..............................................................................*/
/* SD CARD */
#define LOG_FILENAME	"datalog.txt"

const int chipSelect = 4;
File dataFile;

/* END SDCARD */
/*..............................................................................*/

/*..............................................................................*/
/* BMP085 */
#define BMP085_ADDRESS  0x77

const unsigned char OSS = 0;  // Oversampling Setting

// Calibration values
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5; 

short temperature;
long pressure;

/* END BMP085 */
/*..............................................................................*/


//=======================================================
// Initialize
//=======================================================
void setup() {
   Serial.begin(9600);	// Init serial
   initIO();
   initSdCard();
   initBmp085();
}

void initIO() {
	Serial.print("Initializing I/O...");
	pinMode(PIN_ANEMOMETER, INPUT);
	digitalWrite(PIN_ANEMOMETER, HIGH);
	attachInterrupt(0, countAnemometer, FALLING);
	attachInterrupt(1, countRainmeter, FALLING);
	nextCycle = millis() + MSECS_CYCLE;
	Serial.println("I/O initialized.");
}

void initSdCard() {
	Serial.print("Initializing SD card...");
	// see if the card is present and can be initialized:
	if (!SD.begin(chipSelect)) {
		Serial.println("Card failed, or not present");
		// don't do anything more:
		return;
	}
	Serial.println("card initialized.");
}

void initBmp085(){
	Serial.print("Initializing BMP085...");
	// Init i2c
	Wire.begin();
	// Perform calibration
	bmp085Calibration();
	Serial.print("BMP085 calibrated.");
}

// open the file. note that only one file can be open at a time,
// so you have to close this one before opening another.
void openLogFile() {
	Serial.print("Opening sd card file...");
	dataFile = SD.open(LOG_FILENAME, FILE_WRITE);
	if (!dataFile) {
		Serial.print("Error opening sd card file...");
	}
}

void writeWeatherData() {
	String data = weatherData.windSpeed + "," 
		+ weatherData.windDir + "," 
		+ weatherData.rain + "," 
		+ weatherData.temperature + "," 
		+ weatherData.pressure;
	dataFile.println(data);
}

//=======================================================
// Main loop.
//=======================================================
void loop() {
   time = millis();

   if (time >= nextCycle) {
		calcWindSpeed();
		calcWindDir();
		calcBMP085();
		writeWeatherData();
		nextCycle = time + MSECS_CYCLE;
   }
}

//=======================================================
// Interrupt handler for anemometer. Called each time the reed
// switch triggers (one revolution).
//=======================================================
void countAnemometer() {
   numRevsAnemometer++;
}

//=======================================================
// Find vane direction.
//=======================================================
void calcWindDir() {
	String formattedWindDir = "windDir:";
	int val;
	byte x, reading;

	val = analogRead(PIN_VANE);
	val >>= 2;                        // Shift to 255 range
	reading = val;

	// Look the reading up in directions table. Find the first value
	// that's >= to what we got.
	for (x=0; x < NUMDIRS; x++) {
	  if (adc[x] >= reading)
		 break;
	}
	//Serial.println(reading, DEC);
	x = (x + dirOffset) % 8;   // Adjust for orientation
	Serial.print("  Dir: ");
	Serial.println(strVals[x]);
	formattedWindDir += strVals[x];
	weatherData.windDir = formattedWindDir;
}


//=======================================================
// Calculate the wind speed, and display it (or log it, whatever).
// 1 rev/sec = 1.492 mph
//=======================================================
void calcWindSpeed() {
	String formattedWindSpeed = "windSpeed:";
	int integer, decimal, iSpeed;
	// This will produce mph * 10
	// (didn't calc right when done as one statement)
	long speed = 14920;
	speed *= numRevsAnemometer;
	speed /= MSECS_CYCLE;
	iSpeed = speed;         // Need this for formatting below

	Serial.print("Wind speed: ");
	integer = iSpeed / 10;
	decimal = iSpeed % 10;
	formattedWindSpeed += integer;
	formattedWindSpeed += '.';
	formattedWindSpeed += decimal;
	weatherData.windSpeed = formattedWindSpeed;	
	numRevsAnemometer = 0;        // Reset counter
}

//=======================================================
// Interrupt handler for rainmeter. Called each time the reed
// switch triggers (one tip of bucket).
//=======================================================
void countRainmeter() {
	String rainFormatted = "rain:";
	static unsigned long last_millis = 0;
	unsigned long m = millis();
	if (m - last_millis < 200) {
	// ignore interrupt: probably a bounce problem  
	}else {
		numBuckettip++;  
		rainFormatted += numBuckettip;
		weatherData.rain = rainFormatted;	
		Serial.print(numBuckettip);  
	}
	last_millis = m;
}

//=======================================================
// Calculate temperature and pressure
//=======================================================
void calcBMP085() {
	String temperatureFormatted = "temperature:";
	String pressureFormatted = "pressure:";
	temperature = bmp085GetTemperature(bmp085ReadUT());
	temperatureFormatted += temperature;
	weatherData.temperature = temperatureFormatted;	
	pressure = bmp085GetPressure(bmp085ReadUP());
	pressureFormatted += pressure;
	weatherData.pressure = pressureFormatted;	
	// Logs
	Serial.print("Temperature: ");
	Serial.print(temperature, DEC);
	Serial.println(" *0.1 deg C");
	Serial.print("Pressure: ");
	Serial.print(pressure, DEC);
	Serial.println(" Pa");
	Serial.println();
}

// Stores all of the bmp085's calibration values into global variables
// Calibration values are required to calculate temp and pressure
// This function should be called at the beginning of the program
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1 = bmp085ReadInt(0xB6);
  b2 = bmp085ReadInt(0xB8);
  mb = bmp085ReadInt(0xBA);
  mc = bmp085ReadInt(0xBC);
  md = bmp085ReadInt(0xBE);
}

// Calculate temperature given ut.
// Value returned will be in units of 0.1 deg C
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;
  
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return ((b5 + 8)>>4);  
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;
  
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;
    
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available())
    ;
    
  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  
  return (int) msb<<8 | lsb;
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);
  
  // Wait for data to become available
  while(Wire.available() < 3)
    ;
  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}


