/* 
*  Wireless remote controller with Grayhill 67A joystick and nRF24L01+ radios
*  Based on Teensy Examples
*  July 2018 Robbie Edwards
*/

#include <Wire.h>
#include <SPI.h>
#include <RF24.h>

//int led = LED_BUILTIN;
int x = 0;
int y = 0;
int xy[] = {0,0};

RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};

void setup()
{
//  pinMode(led, OUTPUT);
  Wire.begin();             // join i2c bus 
  Serial.begin(115200);       // start serial for output

  radio.begin();
//  radio.setPALevel(RF_PA_LOW);
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  
}

void loop()
{


//  digitalWrite(led, HIGH);  // briefly flash the LED
  //Read from the Grayhill 67A Joystick
  xy[0] = 0;
  xy[1] = 0;
  Wire.requestFrom(0x40, 2);   // 7 bit address, the read / write bit is appended by Wire lib
  if (Wire.available()) { // slave may send less than requested
    int c = Wire.read();   // receive a byte as character
    if (c >127) xy[0] = c - 256;
    else xy[0] = c;
  }
  if (Wire.available()) { // slave may send less than requested
    int c = Wire.read();   // receive a byte as character
    if (c >127) xy[1] = c - 256;
    else xy[1] = c;
  }
    
  Serial.print(xy[0]);Serial.print("  "); Serial.println(xy[1]);

  if (!radio.write( xy, sizeof(xy) )){
       Serial.println(F("failed"));
  }
  

  
//  digitalWrite(led, LOW);
  delay(20);
}
