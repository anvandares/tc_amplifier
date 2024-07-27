/*
  Repeating WiFi Web Client

 This sketch connects to a a web server and makes a request
 using a WiFi equipped Arduino board.

 created 23 April 2012
 modified 31 May 2012
 by Tom Igoe
 modified 13 Jan 2014
 by Federico Vanzati

 This code is in the public domain.

  Find the full UNO R4 WiFi Network documentation here:
  https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples#wi-fi-web-client-repeating
 */
#include "MAX31855.h"
#include "WiFi.h"
#include <Wire.h>
#include "Button.h"
/*--------------------------------------FOR TIME------------------------------*/

#include "RTC.h"// Include the RTC library
#include <NTPClient.h> //Include the NTP library
#include <WiFiUdp.h>

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

void setup() {
/* -------------------------------------------------------------------------- */  

  //Initialize serial and wait for port to open:

  Serial.begin(9600);

 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
    // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:

  connectToWiFi();
  RTC.begin();
  Serial.println("\nStarting connection to server...");
  timeClient.begin();
  timeClient.update();
  auto timeZoneOffsetHours = 2;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print("Unix time = ");
  Serial.println(unixTime);
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);

  
}


/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */  
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  oven_mode();
  lamp_state();
  read_request();
      
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  
  }

}
void print_temp_time()
{

  int mode=get_mode();
  int temp=get_temp();

  char reportTemp[1020];
  RTCTime currentTime;
  RTC.getTime(currentTime); 

  String currentTimeS=String(currentTime);
  char currentTimeStr[25];
 
 
  currentTimeS.toCharArray(currentTimeStr, 25);
  Serial.println(currentTimeS);
  Serial.println(currentTimeStr);


  sprintf(reportTemp, "GET /report/mode/%d/temp/%d/time/%s HTTP/1.1\n", mode, temp, currentTimeStr); //Copies state to string query.
  Serial.println(reportTemp);
  client.println(reportTemp);
    
 
}
// this method makes a HTTP connection to the server:
/* -------------------------------------------------------------------------- */
void httpRequest() {
/* -------------------------------------------------------------------------- */  
  // close any connection before send a new request.
  // This will free the socket on the NINA module
  client.stop();
  char buffer[100]; //For printing host 

  // if there's a successful connection:
  if (client.connect(server, port)) {
    
    Serial.println("\nconnecting...");
    
    // send the HTTP GET request:
  
    print_temp_time();
    client.println("GET / HTTP/1.1");
    
    sprintf(buffer, "Host: %s:/%d HTTP/1.1\n", servStr, port); 
    client.print(buffer); //PRINTS HOST  
   
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();


    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    delay(5000);
  }
}

