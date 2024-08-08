const int buttonPin = 3;  // the number of the pushbutton pin
const int ledPin = 2;  // the pin that the LED is attached to

int buttonPushCounter = 0;  // counter for the number of button presses
int lastButtonState = 0;    // previous state of the button
int buttonState=0; // the current reading from the input pin
bool boolState=false; //Returns true if the button is pushed once and then remains true. 


void oven_mode() //Reads the buttonstate
{
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) 
  {

    // if the state has changed, increment the counter
    if (buttonState == HIGH) 
    { 
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println();
      Serial.println("Oven is set to: ");
      Serial.println(buttonPushCounter);

     if (buttonPushCounter>8) //Oven can be set between 1-8
      {
        buttonPushCounter=0;      
       
      }
      boolState=true; //if button is pressed once. 

    } 
    // Delay a little bit to avoid bouncing
    delay(50);
    
  }

  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;

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
