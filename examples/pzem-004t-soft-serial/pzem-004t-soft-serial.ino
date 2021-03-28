/*
 * NOTE!
 * There is no way to reset energy by software.
 * You can reset it manually by pressing button, but SerialPort should NOT be used at that moment.
 */
#include "pzem-004t.h"

PZEM_004T pzem(10,11);//   Attach new pzem to 10th and 11th pins of Arduino.
/*
* On ESP-01 you can use GPIOs 0 and 2;
* On ESP-12F you may use GPIOs 0,1,2,3,4,5 and 12,13,14,15
*/

void setup() 
{
  Serial.begin(9600);
  while(!pzem.setAddress(192,168,1,1));
}

void loop()
{
  Serial.println();
  
  float V = pzem.getVoltage();
  if (V > 0) Serial.println("V = " + (String)V + " Volts");

  float I = pzem.getCurrent();
  if (I > 0) Serial.println("I = " + (String)I + " Amps");
  
  float P = pzem.getPower();
  if (P > 0) Serial.println("P = " + (String)P + " W");
  
  float E = pzem.getEnergy();
  if (E > 0) Serial.println("E = " + (String)E + " W*h");
    
  if(pzem.setAlarm(10)) Serial.println("Alarm set OK."); // [ 1 <= kW <= 22 ]
  else Serial.print("Alarm set fail.");
  
  delay(1000);
}





