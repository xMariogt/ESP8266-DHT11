#include <DHT.h>
#include <DHT_U.h>
#include <SevSeg.h>


DHT dht(A0, DHT11);
float tempc;
SevSeg sevseg; //Instantiate a seven segment controller object

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
  dht.begin(); 
  byte numDigits = 4;   
  byte digitPins[] = {2, 3, 4, 5}; //Digits: 1,2,3,4 <--put one resistor (ex: 220 Ohms, or 330 Ohms, etc, on each digit pin)
  byte segmentPins[] = {6, 7, 8, 9, 10, 11, 12, 13}; //Segments: A,B,C,D,E,F,G,Period
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(10); //Note: 100 brightness simply corresponds to a delay of 2000us after lighting each segment. A brightness of 0 
                            //is a delay of 1us; it doesn't really affect brightness as much as it affects update rate (frequency).
                            //Therefore, for a 4-digit 7-segment + pd, COMMON_ANODE display, the max update rate for a "brightness" of 100 is 1/(2000us*8) = 62.5Hz.
                            //I am choosing a "brightness" of 10 because it increases the max update rate to approx. 1/(200us*8) = 625Hz.
                            //This is preferable, as it decreases aliasing when recording the display with a video camera....I think.
}

void loop()
{
  //local vars
  tempc = dht.readTemperature();

  Serial.println("Celsius: " +String(tempc));
  sevseg.setNumberF(tempc, 2);

  sevseg.refreshDisplay(); // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right
}







