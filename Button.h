
  // this constant won't change:
const int buttonPin = 2;  // the pin that the pushbutton is attached to
const int ledPin = 13;    // the pin that the LED is attached to

// Variables will change:
int buttonPushCounter = 0;  // counter for the number of button presses
int buttonState = 0;        // current state of the button
int lastButtonState = 0;    // previous state of the button

int get_mode();
void oven_mode();


void oven_mode()
{
 
// read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  if (buttonState != lastButtonState) 
  {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) 
    {
      // if the current state is HIGH then the button went from off to on:
      buttonPushCounter++;
      Serial.println("Oven is set to: ");
      Serial.println(buttonPushCounter);
      if (buttonPushCounter>7) //Oven can be set between 1-8
      {
        buttonPushCounter=0;
      }
    } 
    // Delay a little bit to avoid bouncing
    delay(50);
    
  }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;



}
int get_mode()
{

  return buttonPushCounter;

}