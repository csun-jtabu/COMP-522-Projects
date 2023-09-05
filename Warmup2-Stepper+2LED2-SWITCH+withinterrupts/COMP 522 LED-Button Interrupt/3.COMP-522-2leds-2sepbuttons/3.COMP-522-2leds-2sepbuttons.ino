/*
Jaztin Tabunda
Warmup Project 2 - 2 LEDs and 2 Separate Buttons
COMP 522 - V. Bhatt
3-31-2023
*/

int buttonPin = 2; //pin number that takes input from button
int redLedPin = 4; //pin number that outputs to red led
int buttonPin2 = 3; //pin number that takes input from button2
int greenLedPin = 5; //pin number that outputs to green led

int toggleState; //checks to see if red light is on or off
int toggleState2; //checks to see if green light is on or off
int lastButtonState = 1; //since this is a pull up resistor, button goes from HIGH (1) to LOW (0)
int lastButtonState2 = 1; //since this is a pull up resistor, button2 goes from HIGH (1) to LOW (0)
long unsigned int lastPress; //this variable is going to keep track of the milliseconds since the last time the button was pressed
long unsigned int lastPress2; //this variable is going to keep track of the milliseconds since the last time button2 was pressed
volatile int buttonFlag; //this variable is to check if the button is pressed or not.
volatile int buttonFlag2; //this variable is to check if the button2 is pressed or not.

int debounceTime = 20; //this is to give the program rest time for input so it's stable

void setup()
{
  pinMode(redLedPin, OUTPUT); //sets up red led as output
  pinMode(greenLedPin, OUTPUT); //sets up green led as output
  pinMode(buttonPin, INPUT_PULLUP); //sets up button as input
  pinMode(buttonPin2, INPUT_PULLUP); //sets up button2 as input
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button_red, CHANGE); //this sets up the interrupt for the button so that it runs the ISR method when there is a change
  attachInterrupt(digitalPinToInterrupt(buttonPin2), ISR_button_green, CHANGE); //this sets up the interrupt for the button2 so that it runs the ISR method when there is a change
}

void loop()
{

  //this if statement is basically a delay without delaying the entire program
  //as long as the button press happens after a set amount of time (debounceTime) and the buttonFlag is equal to 1
  //the only time buttonFlag is equal to 1 is when there is a change interrupt on the button (i.e. when you press the button)
  if( ((millis() - lastPress) > debounceTime && buttonFlag))
  {

    lastPress = millis();

    //first button and red led
    //if you press button (since button is a pull up resistor, the change is from 1 to 0)
    if(digitalRead(buttonPin) == 0 && lastButtonState == 1)
    {
      toggleState =! toggleState; //toggleState controls the red led bulb from on to off and vice versa
      digitalWrite(redLedPin, toggleState);
      lastButtonState = 0; //the current button state turns into the last button state the next time it's looped
    }
    else if(digitalRead(buttonPin) == 1 && lastButtonState == 0)
    {
      lastButtonState = 1; //the current button state turns into the last button state the next time it's looped
    }
      buttonFlag = 0; //turns it to zero so it's not going to run the if statement again until the button is pressed again.
  }
  else if( ((millis() - lastPress2) > debounceTime && buttonFlag2))  
  {
    lastPress2 = millis();

    //second button and green led
    if(digitalRead(buttonPin2) == 0 && lastButtonState2 == 1)
    {
      toggleState2 =! toggleState2; //toggleState controls the green led bulb from on to off and vice versa
      digitalWrite(greenLedPin, toggleState2);
      lastButtonState2 = 0; //the current button state turns into the last button2 state the next time it's looped
    }
    else if(digitalRead(buttonPin2) == 1 && lastButtonState2 == 0)
    {
      lastButtonState2 = 1; //the current button2 state turns into the last button2 state the next time it's looped
    }
      buttonFlag2 = 0; //turns it to zero so it's not going to run the if statement again until button2 is pressed again.
  }

}

void ISR_button_red()
{
  buttonFlag = 1; //if button is pressed, it allows the if statement in the main loop to be ran
}

void ISR_button_green()
{
  buttonFlag2 = 1; //if button2 is pressed, it allows the if statement in the main loop to be ran
}