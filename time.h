
/*--------------------------------------FOR TIME------------------------------*/

#include "RTC.h"// Include the RTC library
#include <NTPClient.h> //Include the NTP library
#include <WiFiUdp.h>


WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);
unsigned long timerStarted=0UL; //Placed it here so that the variable can be reached in get_timer()

String get_time() //Return the time 
{
  char cstr[20];
  RTCTime currentTime;
  RTC.getTime(currentTime); 
  int year =currentTime.getYear();
  int month=int(currentTime.getMonth())+1;
  int day=currentTime.getDayOfMonth();
  int hour=currentTime.getHour();
  int minute=currentTime.getMinutes();
  int seconds=currentTime.getSeconds();  

  sprintf_P(cstr, (PGM_P)F("%04d-%02d-%02dT%02d:%02d:%02d"), year, month, day, hour, minute, seconds);
  
  return cstr;
  }
void set_timer()
{
  timerStarted=millis();//Note millis that have passed since program started to when timer was set.

}
String get_timer()
{
  char gt[20]; 
  unsigned long timerStopped=millis(); //Note millis that have passed since program started to when timer was stopped
  unsigned long timerTime = (timerStopped - timerStarted)/1000; //Total time between timerStarted and timerStopped in seconds
  unsigned int timerSec = timerTime % 60; //Sec
  unsigned int timerMin = (timerTime/60)%60;  //Min
  unsigned int timerHour = (timerTime / (60 * 60)); //I won't bother getting days, one can hope that the temperature will stabilize whitin 24-hours. 
 

  sprintf_P(gt,(PGM_P)F("%02d:%02d:%02d"), timerHour, timerMin, timerSec); 
  
  //Prints for debugging.
  Serial.println("timer stopped at:");
  Serial.println(gt);

  return gt;
  
  
}
static bool _delay(unsigned long timeStamp, unsigned long hourGlass){ /*Checks if time has passed between readings. Takes timeStamp (time when reading was made), 
                                                                        and hourGlass (time you want to have passed before next reading is made.) as arguments.*/
    if (millis() - timeStamp > hourGlass) 
    {       
            return (true);               
    }
    else
    {   
        return (false);
    }
  
}
