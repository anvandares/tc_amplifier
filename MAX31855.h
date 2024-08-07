#include <SPI.h>
#include "Adafruit_MAX31855.h"

// Default connection is using software SPI, but comment and uncomment one of
// the two examples below to switch between software SPI and hardware SPI:

// Example creating a thermocouple instance with software SPI on any three
// digital IO pins.
#define MAXDO 5
#define MAXCS 4
#define MAXCLK 3



// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

// Example creating a thermocouple instance with hardware SPI
// on a given CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS);

// Example creating a thermocouple instance with hardware SPI
// on SPI1 using specified CS pin.
//#define MAXCS   10
//Adafruit_MAX31855 thermocouple(MAXCS, SPI1);


const int ledPin = 2;  // the pin that the LED is attached to
double c=0;
double get_temp();



static bool temp_state() //Returns false if the Thermocouple is wired incorrectly- 
{
  
   if (isnan(thermocouple.readCelsius())) {
     Serial.println("Thermocouple fault(s) detected!");
     uint8_t e = thermocouple.readError();
     if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
     if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
     if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
     return (false);
  
   }else{
       
     return (true);
        
    }

}
void print_temp()//Prints the internal temp of the module and the temp of the thermocouple 
{  
  if(temp_state()) //if it's wired correctly
  {
     // basic readout test, just print the current temp
    Serial.print("Internal Temp = ");
    Serial.println(thermocouple.readInternal());  
   
    Serial.print("C = ");
    Serial.println(get_temp());

  }
}
double get_temp() //Reads the temp if the thermocouple is wired correctly
{
  if(temp_state())
  {
     c = thermocouple.readCelsius();  
     return c;  

  } 
 
}
bool lamp_state(double a, double b) //Checks if temp has changed
{

  if((b-a)>1||(a-b)>1)             //If it differs more than 2 degrees between the readings the temp is still rising?
  {
    digitalWrite(ledPin, HIGH); //Turns LED on if the temp is still rising
    return (true);
  
  }
  else
  {
    digitalWrite(ledPin, LOW); //If temp has stabilized the LED is i turned of.
    return (false);
     
  }
     
  

}
















