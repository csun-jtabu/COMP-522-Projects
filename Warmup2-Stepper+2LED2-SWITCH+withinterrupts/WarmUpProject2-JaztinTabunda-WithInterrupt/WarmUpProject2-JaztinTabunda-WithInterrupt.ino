/*
Jaztin Tabunda
Warm-Up Project 2 - Step Motor with Button Control using Interrupts
COMP 522 - V. Bhatt
3-31-2023
*/


#include <Stepper.h>

//according to my research on the motor I'm using
//the stepsPerRevoltion on Rohs step motor 28byj-48 is 2048 
const int stepsPerRevolution = 2048;

// setting up the stepper library on pins 8 through 11:
// 8, 10, 9, 11 is for the driver and motor i'm using
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); 

int currVelocity; //the current speed that the motor will run at
int stopPrint = 1; //the delay when switching states

int toggleState; //turns on stepper motor
volatile int buttonFlag; //flag for button
volatile int button2Flag; //flag for button2
int lastPress; //last time button was pressed
int lastPress2; //last time button2 was pressed

int oldButton1State = 0; // INPUT1 state before pressing button
int newButton1State = 0; // INPUT1 state after pressing button
const int button1Pin = 2; // INPUT1 pin

int oldButton2State = 0; // INPUT2 state before pressing button
int newButton2State = 0; // INPUT2 state after pressing button
const int button2Pin = 3; // INPUT2 pin

int debounceTime = 50; //rest time for debounce

//enumeration of the states that we'll have
enum States{INITIAL, STOP, VEL1, VEL2, VEL3, VEL4}; 

States currState = INITIAL; //this is the current state of the program. starts at initial state
States prevState; //this is the previous state of the program (used when Stop)

void setup() {
  Serial.begin(9600); // Serial.begin gets the arduino ready to communicate with text output
  pinMode(button1Pin, INPUT); // sets up pin 2 as input for button1
  pinMode(button2Pin, INPUT); // sets up pin 3 as input for button2
  attachInterrupt(digitalPinToInterrupt(button1Pin), ISR_button1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button2Pin), ISR_button2, CHANGE);
}

void loop() {

  //state machine powered by this switch statement
  switch(currState)
  {
    //the starting state of the program
    case INITIAL:
    stateInitial(); //this is a void method call to the code for the initial state
    break;

    //the state of the program where the motor pauses revolutions
    //and can possibly reset the current velocity/speed of the motor
    case STOP:
    stateStop(); //this is a void method call to the code for the stop state
    break;

    //the state of the program with speed 1 = 3
    case VEL1:
    stateVel1(); //this is a void method call to the code for the Speed1 state
    break;

    //the state of the program with speed 2 = 6
    case VEL2:
    stateVel2(); //this is a void method call to the code for the Speed2 state
    break;

    //the state of the program with speed 3 = 9
    case VEL3:
    stateVel3(); //this is a void method call to the code for the Speed3 state
    break;

    //the state of the program with speed 4 = 12 (Max Velocity)
    case VEL4:
    stateVel4(); //this is a void method call to the code for the Speed4 state
    break;

  }

}

//this is the code where the program starts
void stateInitial()
{
  if(((millis() - lastPress) > debounceTime) && buttonFlag)
  {
    newButton1State = digitalRead(button1Pin); //reads input from button1
    //makes sure the button changes from LOW to HIGH in order to properly change
    //if we only did buttonstate == high, it won't work because of 'chattering'
    //i.e. the button will read many inputs at one time and malfunction due to
    //debouncing
    if(oldButton1State != newButton1State) 
    {
      lastPress = millis();
      oldButton1State = newButton1State;
      if(newButton1State == LOW)
      {
        currState = VEL1; //changes initial to first speed of motor
      }
    }
    buttonFlag = 0;
  }
  else
  {
    if(currVelocity != 0)
    {
      currVelocity = 0; //we start with 0 rpm and no steps so motor doesn't move
      Serial.println("initial"); //this is to tell us which state we are currently in
      //as long as we don't press button1 or button2, we will remain in this state
    }
  }
}

//this is the state that the program goes to after we press button2
//notice there is no .step() function call to make sure motor doesn't run
void stateStop()
{
  //as long as we don't press button1 or button2, we will remain in this state
  if(((millis() - lastPress) > debounceTime) && buttonFlag)
  {
    lastPress = millis();
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    if(oldButton1State != newButton1State) //press input/button 1
    {
      oldButton1State = newButton1State; //explained before, this is for debouncing
      if(newButton1State == LOW)
      {
        currState = prevState; //goes back to the state it came from
        prevState = STOP;
        Serial.println("Previous State");
      }
    }
    buttonFlag = 0;
  }
  else if(((millis() - lastPress2) > debounceTime) && button2Flag)
  {
    lastPress2 = millis();
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if(oldButton2State != newButton2State)
    {
      oldButton2State = newButton2State;
      if(newButton2State == LOW)
      {
        currState = INITIAL; //goes to initial (i.e. resets velocity back to 0)
      }
    }
    button2Flag = 0;
  }
  else
  {
    if(stopPrint == 1)
    {
      Serial.println("Stop"); //this is to tell us which state we are currently in
      stopPrint = 0;
    }
  }
}

void stateVel1()
{
  if(((millis() - lastPress) > debounceTime) && buttonFlag)
  {
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    if(oldButton1State != newButton1State) //if INPUT1 is pressed
    {
      lastPress = millis();
      oldButton1State = newButton1State; //for debouncing again
      if(newButton1State == LOW)
      {
        currState = VEL2; //goes to a faster speed if input1 is pressed
      }
    }
    buttonFlag = 0;
  }
  else if(((millis() - lastPress2) > debounceTime) && button2Flag)
  {
    stopPrint = 1;
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if(oldButton2State != newButton2State)
    {
      lastPress2 = millis();
      oldButton2State = newButton2State;
      if(newButton2State == LOW)
      {
        prevState = VEL1; //saves the state so we can go back to it from the stop state
        currState = STOP; //goes to the stop state
      }
    }
    button2Flag = 0;
  }
  else
  {
    if(currVelocity != 3)
    {
      currVelocity = 3; //first speed = 3rpm
      myStepper.setSpeed(currVelocity); //sets the speed of the motor
      Serial.println("vel1"); //states what state we are in
    }
    else
    {
      myStepper.step(1);
    }
  }
    
}

void stateVel2()
{
  //as long as we don't press button1 or button2, we will remain in this state
  if(((millis() - lastPress) > debounceTime) && buttonFlag)
  {
    //for debouncing again
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    if(oldButton1State != newButton1State) //if INPUT1 is pressed
    {
      lastPress = millis();
      oldButton1State = newButton1State;
      if(newButton1State == LOW)
      {
        currState = VEL3; //goes to a faster speed if input1 is pressed
      }
    }
    buttonFlag = 0;
  }
  else if(((millis() - lastPress2) > debounceTime) && button2Flag)
  {
    stopPrint = 1;
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if(oldButton2State != newButton2State)
    {
      lastPress2 = millis();
      oldButton2State = newButton2State;
      if(newButton2State == LOW)
      {
        prevState = VEL2; //saves current state so we can go back to it from the stop state
        currState = STOP; //goes to the stop state
      }
      //delay(del); //delay program for half a sec after state change
    }
    button2Flag = 0;
  }
  else
  {
    if(currVelocity != 6)
    {
      currVelocity = 6; //second speed = 6rpm
      myStepper.setSpeed(currVelocity); //sets the speed of the motor
      Serial.println("vel2"); //states what state we are in
    }
    else
    {
      myStepper.step(1); //steps once and loops. if we use steps per revolution, it will try to complete one full revolution before being able to read input
    }
  }
}

void stateVel3()
{
    //as long as we don't press button1 or button2, we will remain in this state
  if(((millis() - lastPress) > debounceTime) && buttonFlag)
  {
    //again, for debouncing
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    if(oldButton1State != newButton1State) //if INPUT1 is pressed
    {
      lastPress = millis();
      oldButton1State = newButton1State; 
      if(newButton1State == LOW)
      {
        currState = VEL4; //if input1 is pressed, the program goes to a faster speed
      }
    }
    buttonFlag = 0;
  }
  else if(((millis() - lastPress2) > debounceTime) && button2Flag)
  {
    stopPrint = 1;
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if(oldButton2State != newButton2State) //if INPUT2 is pressed
    {
      lastPress2 = millis();
      oldButton2State = newButton2State; 
      if(newButton2State == LOW)
      {
        prevState = VEL3; //saves current state so we can go back from stop state
        currState = STOP; //goes to stop state
      }
    }
    button2Flag = 0;
  }
  else
  {
    if(currVelocity != 9)
    {
      currVelocity = 9; //third speed = 9rpm
      myStepper.setSpeed(currVelocity); //sets the new speed of the motor
      Serial.println("vel3"); //to explicitly tell which state we are in
    }
    else
    {
      myStepper.step(1); //steps once and loops. if we use steps per revolution, it will try to complete one full revolution before being able to read input
    }
  }
}

void stateVel4()
{
  //will stay in state as long as we don't press button2. instructions don't say anything about cycling back to the first state with button1
  if(((millis() - lastPress2) > debounceTime) && button2Flag)
  {
    stopPrint = 1;
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if(oldButton2State != newButton2State) //if INPUT2 is pressed
    {
      lastPress2 = millis();
      oldButton2State = newButton2State; //for debouncing
      if(newButton2State == LOW)
      {
        prevState = VEL4; //saves current state so we can go back from stop state
        currState = STOP; //goes to stop state
      }
    }
    button2Flag;
  }
  else
  {
    if(currVelocity != 12)
    {
      currVelocity = 12; //fourth and final speed = 12
      myStepper.setSpeed(currVelocity); //sets new speed of motor
      Serial.println("vel4"); //prints out which state we are in
    }
    else
    {
      myStepper.step(1); //steps once and loops. if we use steps per revolution, it will try to complete one full revolution before being able to read input
    }
  }
}

void ISR_button1()
{
  buttonFlag = 1;
}

void ISR_button2()
{
  button2Flag = 1;
}