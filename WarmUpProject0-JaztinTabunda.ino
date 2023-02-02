/*
Jaztin Tabunda
Warm-Up Project 0 - Button
COMP 522 - V. Bhatt
1-29-2023
*/

const int buttonPinNum = 2; //pin number for the button
const int ledPinNum = 13; //pin number for the led we are lighting up

int buttonCurrentState = 0; //checks if the button is pressed or not. (0 or 'LOW') if not pressed, (1 or 'HIGH') if pressed
int currentDelay = 1000; //current blink delay is 1000ms = 1s = 1hz
int resetDelay = 125; //this is the last/fastest delay the specs ask for 125ms = 0.125s = 8hz

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPinNum, INPUT); //sets up button as something that is taking in input
  pinMode(ledPinNum, OUTPUT); //sets up led on Arduino board as something that will show output 
}

void loop() {
  // put your main code here, to run repeatedly:
  
  buttonCurrentState = digitalRead(buttonPinNum); //checks the current state of the button (on/off)

  if(buttonCurrentState == HIGH) //if the button is pressed
  {
    currentDelay = currentDelay / 2; //the currentDelay will be halfed each time the button is pressed (1 -> 0.5 -> 0.25 -> 0.125) 
    if(currentDelay < resetDelay) //if the currentDelay is passed 8hz, it will reset back to 1hz
    {
      currentDelay = 1000; 
    }
    delay(5000); //blink stops for 5 seconds to show that it changed blink frequencies
  }
  else //if the button isn't pressed, it will continue to blink with the currentDelay
  {
      digitalWrite(ledPinNum, HIGH); //turns led on
      delay((currentDelay / 2)); //delay half the currentDelay 
      digitalWrite(ledPinNum, LOW); //turns led off
      delay((currentDelay / 2)); //delay half the currentDelay
      //delay off + delay on = currentDelay
  }
      

}
