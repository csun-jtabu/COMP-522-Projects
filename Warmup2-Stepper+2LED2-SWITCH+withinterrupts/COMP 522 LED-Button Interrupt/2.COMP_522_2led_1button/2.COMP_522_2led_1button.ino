/*
Jaztin Tabunda
Warmup Project 2 - 2 LED and 1 Button
COMP 522 - V. Bhatt
3-31-2023
*/

int buttonPin = 2; //pin number that takes input from button
int redLedPin = 4; //pin number that outputs to red led
int greenLedPin = 5; //pin number that outputs to green led

int toggleState; //checks to see if red light is on or off
int toggleState2; //checks to see if green light is on or off
int lastButtonState = 1; //since this is a pull up resistor, button goes from HIGH (1) to LOW (0)
long unsigned int lastPress; //this variable is going to keep track of the milliseconds since the last time the button was pressed
volatile int buttonFlag; //this variable is to check if the button is pressed or not.
int pressCount = 0; //the number of times the button is pressed
int debounceTime = 20; //this is to give the program rest time for input so it's stable

void setup()
{
  pinMode(redLedPin, OUTPUT); //sets up red led as output
  pinMode(greenLedPin, OUTPUT); //sets up green led as output
  pinMode(buttonPin, INPUT_PULLUP); //sets up button as input
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button, CHANGE); //this sets up the interrupt for the button so that it runs the ISR method when there is a change
}

void loop()
{
  //this if statement is basically a delay without delaying the entire program
  //as long as the button press happens after a set amount of time (debounceTime) and the buttonFlag is equal to 1
  //the only time buttonFlag is equal to 1 is when there is a change interrupt on the button (i.e. when you press the button)
  if( ((millis() - lastPress) > debounceTime) && buttonFlag)
  {

    lastPress = millis();

    //first button and red led
    //if you press button (since button is a pull up resistor, the change is from 1 to 0)
    if(digitalRead(buttonPin) == 0 && lastButtonState == 1)
    {
      pressCount++; //everytime button is pressed it increments the pressCount
      if(pressCount == 1) //red on, green off
      {
        toggleState =! toggleState; //toggleState controls the red led bulb from on to off and vice versa
        digitalWrite(redLedPin, toggleState); 
      }
      else if(pressCount == 2) //green on, red off
      {
        toggleState =! toggleState; //toggleState controls the red led bulb from on to off and vice versa
        toggleState2 =! toggleState2; //toggleState controls the green led bulb from on to off and vice versa
        digitalWrite(redLedPin, toggleState); 
        digitalWrite(greenLedPin, toggleState2);
      }
      else if(pressCount == 3) //both on
      {
        toggleState =! toggleState; //toggleState controls the red led bulb from on to off and vice versa
        digitalWrite(redLedPin, toggleState); 
      }
      else //both off
      {
        toggleState = 0; //turns off leds
        toggleState2 = 0;
        digitalWrite(redLedPin, toggleState); 
        digitalWrite(greenLedPin, toggleState2);
        pressCount = 0; //resets presscount
      }
      lastButtonState = 0; //the current button state turns into the last button state the next time it's looped
    }//after you press the button, it resets the state (this is for debouncing)
    else if(digitalRead(buttonPin) == 1 && lastButtonState == 0)
    {
      lastButtonState = 1; //the current button state turns into the last button state the next time it's looped
    }
      buttonFlag = 0; //turns it to zero so it's not going to run the if statement again until the button is pressed again.
  }
  
}

void ISR_button()
{
  buttonFlag = 1; //when the button is pressed, you can run the if statement now
}