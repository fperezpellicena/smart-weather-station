#ifndef BMP085_H_
#define BMP085_H_

#define BMP085_R 0xEF
#define BMP085_W 0xEE
#define OSS 0
#define FALSE	0
#define TRUE	-1

void BMP085_Calibration(void);

short bmp085ReadShort(unsigned char address);
long bmp085ReadTemp(void);
long bmp085ReadPressure(void);
void bmp085Convert(long * temperature, long * pressure);


#endif /* BMP085_H_ */
