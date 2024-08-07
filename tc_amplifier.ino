
#include "MAX31855.h"
#include "Button.h"
#include "time.h"
#include "led_matrix.h"
#include "WiFi.h"

#include <Wire.h>
unsigned long timeStamp = millis(); //Notes the time when program started.
double firstTemp=0; //For checking if the temp has risen. 
double secondTemp=0; //
unsigned long lastConnectionTime = 0;  // last time you connected to the server, in milliseconds
String servStr= server.toString(); //To add IP-adress to sprintf and print to client 
bool boolSetTimer=false;  //So that timer only is set once
bool boolGetTimer=false; //So that timer only is stopped once
bool delayRunning = false; //delay between temperature readings.
int startFrame=10; //Prints "Set." to built in display

void setup() {
/* -------------------------------------------------------------------------- */  


   // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  // initialize serial communication:
    //Initialize serial and wait for port to open:
  Serial.begin(9600);

 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 

  connectToWiFi();
  Serial.println("\nStarting connection to server...");
  
  RTC.begin();
  timeClient.begin();
  timeClient.update();
  auto timeZoneOffsetHours = 2;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print("Unix time = ");
  Serial.println(unixTime);
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);
  Serial.print("NOW time = ");
  Serial.println(timeToSet);
  matrix.begin();
  delayRunning = true; //MÅSTE DEN VA HÄR?

}
/* -------------------------------------------------------------------------- */
void loop() {
/* -------------------------------------------------------------------------- */  

  read_request(); 
  do{    
    oven_mode(); //reads buttonstate

    if(boolState){ //If button is pushed, break loop to move on from "start mode". 
      break;
    }   
    if(_delay(timeStamp, 1500UL)){

      if(startFrame==10){
        print_frame(startFrame);
        startFrame++;
      }
      else if(startFrame==11){
        print_frame(startFrame);
        startFrame--;
      }
      timeStamp = millis();

    }
   
  }while(!boolState);  //Checks if button has been pushed at all.

    print_frame(buttonPushCounter);   //prints the oven mode to the built in led display.


    if(_delay(timeStamp, 15000UL)){ //Reads the temp every fifteen seconds
      
      if(delayRunning){ //To switch between the two readings
        firstTemp=get_temp(); 
       
        Serial.println("firstTemp: ");
        Serial.print(firstTemp); 
        Serial.println("at: ");
        Serial.print(get_time()); 
        delayRunning=false;
       
      }
      else{      
        secondTemp=get_temp();           
        Serial.println("secondTemp: ");
        Serial.println(secondTemp); 
        Serial.println("at: ");
        Serial.print(get_time()); 
        delayRunning=true;
      }
      
      timeStamp = millis(); //updates the time when the last reading was made
           
    } 
    if(!boolSetTimer&&secondTemp>0){ //If the timer is not set and there has been a second reading of temp. 

      if(lamp_state(firstTemp, secondTemp)){ //If the temp is still rising.           
        boolSetTimer=true;    //So that timer is set only once
        Serial.println("SET"); 
        set_timer(); //Notes the time when timer was set.

        }
    }else if(boolSetTimer&&!boolGetTimer){ //if timer is set but not stopped.

      if(!lamp_state(firstTemp, secondTemp)){//if the two temperature readings are the same +-2 c
        Serial.println("GET"); 
        get_timer(); //Notes the time when t
        boolGetTimer=true; //stop timer     
        }
    }

  
  if(_delay(lastConnectionTime,10000UL)){  // if ten seconds have passed since your last connection,
      httpRequest();                                // then connect again and send data:
      lastConnectionTime = millis();   // notes the time that the connection was made:
        

  }
}

String client_print()
{
  int temp=int(get_temp());
  char timeStr[25];
  String time=get_time();
  time.toCharArray(timeStr, 25);
  
  char reportTemp[1020];


  sprintf(reportTemp, "GET /report/mode/%d/temp/%d/time/%s HTTP/1.1\n", buttonPushCounter, temp, timeStr); //Copies state to string query.
  Serial.println(reportTemp);

  return reportTemp;

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
    client.println(client_print());
    client.println("GET / HTTP/1.1");
    
    sprintf(buffer, "Host: %s:/%d HTTP/1.1\n", servStr, port); 
    client.print(buffer); //PRINTS HOST  
   
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    
    
    client.println();


   
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
   
  }
}

