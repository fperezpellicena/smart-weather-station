/*   
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
 
 UNO DE LOS PROBLEMAS DE ESTE PROGRAMA ES QUE ESTA CONSTANTEMENTE CONTANDO 
 LOS PULSOS DEL ANEMOMETRO CON LA INTERRUPCION. ESO PARA MEDIR CADA POCO 
 TIEMPO ESTA BIEN PERO SI MIDES CADA HORA POR EJEMPLO, NO. ES MEJOR QUE A
 LA HORA SE ACTIVE Y MIDA LOS PULSOS EN 5 SEGUNDOS. ASI TENDREMOS LA VELOCIDAD
 EN ESE MOMENTO Y NO UNA MEDIA DE TODA LA HORA.
 */

#define ulong unsigned long

#define PIN_ANEMOMETER  2     // Digital 2

// How often we want to calculate wind speed or direction
#define MSECS_CALC_WIND_SPEED 5000


volatile int numRevsAnemometer = 0; // Incremented in the interrupt
ulong nextCalcSpeed;                // When we next calc the wind speed
ulong time;                         // Millis() at each start of loop().

//=======================================================
// Initialize
//=======================================================
void setup() {
  // Init arduino i/o
  pinMode(PIN_ANEMOMETER, INPUT);
  digitalWrite(PIN_ANEMOMETER, HIGH);  //Aqui le estas metiendo una resistencia de pull-up al ser un input. No se que efecto puede tener.
  attachInterrupt(0, countAnemometer, FALLING); //La interrupcion 0 es el pin 2 de la placa de arduino
  nextCalcSpeed = millis() + MSECS_CALC_WIND_SPEED;
}

//=======================================================
// Main loop.
// TODO Emplear modo de bajo consumo
//   set_sleep_mode(SLEEP_MODE_PWR_DOWN);   
//   sleep_enable();          
//   sleep_mode();
//   sleep_disable();
//=======================================================
void loop() {
  time = millis();

  if (time >= nextCalcSpeed) {
    calcWindSpeed();
    nextCalcSpeed = time + MSECS_CALC_WIND_SPEED;
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
// Calculate the wind speed, and display it (or log it, whatever).
// 1 rev/sec = 2.4 Km/h
//=======================================================
void calcWindSpeed() {
  int x, iSpeed;
  // This will produce Km/h *100 (esto es as pq la uart no puede mandar FLOAT y hay que mandarlo como integer,
  // en el ordenador habra que coger este nmero y dividirlo entre 100 para sacar Km/h)
  // (No calcula bien si lo haces en una sola sentencia)
  long speed = 240000;
  speed *= numRevsAnemometer;
  speed /= MSECS_CALC_WIND_SPEED;
  iSpeed = speed;         // Need this for formatting below

  Serial.print("Wind speed: ");
  x = iSpeed;
  Serial.print(x);
  
  numRevsAnemometer = 0;        // Reset counter
}



