/* 
*  Wireless remote controller receiver with Grayhill 67A joystick and nRF24L01+ radios
*  Based on Teensy Examples
*  July 2018 Robbie Edwards
*/

#include <SPI.h>
#include <RF24.h>
#include <Servo.h>
#define THRUSTER_PIN 3

RF24 radio(9,10);
int xy[] = {0,0};
Servo m1;
int thruster_out = 90;
int missCount = 11;

byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  m1.attach(THRUSTER_PIN);
  radio.begin();
//  radio.setPALevel(RF_PA_LOW);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();

}

void loop() {
  if (missCount > 10) thruster_out = 90; // If we don't receive anything for 10 cycles, set throttle to neutral
  if( radio.available()){
                                                                    
      while (radio.available()) {                                   
        radio.read( &xy, sizeof(xy) );             
      }
     
      Serial.print(xy[0]);Serial.print("  "); Serial.println(xy[1]);
      thruster_out = map(xy[1],-80,80,8,172);  // map the joystick output to the servo range
      missCount = 0;
   }
   else missCount = missCount + 1;

   if (thruster_out < 90) thruster_out = 90; // Don't allow reverse
   m1.write(thruster_out);
   delay(20);
}
