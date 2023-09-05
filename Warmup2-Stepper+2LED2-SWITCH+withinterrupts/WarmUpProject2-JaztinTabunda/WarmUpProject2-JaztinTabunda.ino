/*
Jaztin Tabunda
Warm-Up Project 2 - Step Motor with Button Control
COMP 522 - V. Bhatt
3-3-2023
*/


#include <Stepper.h>

//according to my research on the motor I'm using
//the stepsPerRevoltion on Rohs step motor 28byj-48 is 2048 
const int stepsPerRevolution = 2048;

// setting up the stepper library on pins 8 through 11:
// 8, 10, 9, 11 is for the driver and motor i'm using
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); 

int currVelocity; //the current speed that the motor will run at
int const del = 500; //the delay when switching states

int oldButton1State = 0; // INPUT1 state before pressing button
int newButton1State = 0; // INPUT1 state after pressing button
const int button1Pin = 2; // INPUT1 pin

int oldButton2State = 0; // INPUT2 state before pressing button
int newButton2State = 0; // INPUT2 state after pressing button
const int button2Pin = 3; // INPUT2 pin

//enumeration of the states that we'll have
enum States{INITIAL, STOP, VEL1, VEL2, VEL3, VEL4}; 

States currState = INITIAL; //this is the current state of the program. starts at initial state
States prevState; //this is the previous state of the program (used when Stop)

void setup() {
  
  Serial.begin(9600); // Serial.begin gets the arduino ready to communicate with text output
  pinMode(button1Pin, INPUT); // sets up pin 2 as input for button1
  pinMode(button2Pin, INPUT); // sets up pin 3 as input for button2
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
  currVelocity = 0; //we start with 0 rpm and no steps so motor doesn't move
  Serial.println("initial"); //this is to tell us which state we are currently in

  //as long as we don't press button1 or button2, we will remain in this state
  while(currState == INITIAL) 
  {
    newButton1State = digitalRead(button1Pin); //reads input from button1
    newButton2State = digitalRead(button2Pin); //reads input from button1
    //makes sure the button changes from LOW to HIGH in order to properly change
    //if we only did buttonstate == high, it won't work because of 'chattering'
    //i.e. the button will read many inputs at one time and malfunction due to
    //debouncing
    if((oldButton1State == LOW) && (newButton1State == HIGH)) 
    {
      currState = VEL1; //changes initial to first speed of motor
      delay(del); //delay program for half a sec after state change
    }
    else
    {
      oldButton1State = newButton1State; //again, these two lines are to prevent
      oldButton2State = newButton2State; //bouncing input
    }
    
  }
    
}

//this is the state that the program goes to after we press button2
//notice there is no .step() function call to make sure motor doesn't run
void stateStop()
{
  Serial.println("Stop"); //this is to tell us which state we are currently in
  
  //as long as we don't press button1 or button2, we will remain in this state
  while(currState == STOP) 
  {
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if((oldButton1State == LOW) && (newButton1State == HIGH)) //press input/button 1
    {
      currState = prevState; //goes back to the state it came from
      delay(del); //delay program for half a sec after state change
    }
    else if((oldButton2State == LOW) && (newButton2State == HIGH)) //press input/button 2
    {
      currState = INITIAL; //goes to initial (i.e. resets velocity back to 0)
      delay(del); //delay program for half a sec after state change
    }
    else
    {
      oldButton1State = newButton1State; //explained before, this is for debouncing
      oldButton2State = newButton2State;
    }
  }
}

void stateVel1()
{
  currVelocity = 3; //first speed = 3rpm
  myStepper.setSpeed(currVelocity); //sets the speed of the motor
  Serial.println("vel1"); //states what state we are in

  //as long as we don't press button1 or button2, we will remain in this state
  while(currState == VEL1)
  {
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if((oldButton1State == LOW) && (newButton1State == HIGH)) //if INPUT1 is pressed
    {
      currState = VEL2; //goes to a faster speed if input1 is pressed
      delay(del); 
    }
    else if((oldButton2State == LOW) && (newButton2State == HIGH)) //if INPUT2 is pressed
    {
      prevState = VEL1; //saves the state so we can go back to it from the stop state
      currState = STOP; //goes to the stop state
      delay(del); //delay program for half a sec after state change
    }
    else //if no inputs are pressed
    {
      oldButton1State = newButton1State; //for debouncing again
      oldButton2State = newButton2State;
      myStepper.step(1); //steps once and loops. if we use steps per revolution, it will try to complete one full revolution before being able to read input 
    }
    
    
  }
}

void stateVel2()
{
  currVelocity = currVelocity + 3; //second speed = 6rpm
  myStepper.setSpeed(currVelocity); //sets the speed of the motor
  Serial.println("vel2"); //states what state we are in

  //as long as we don't press button1 or button2, we will remain in this state
  while(currState == VEL2)
  {
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if((oldButton1State == LOW) && (newButton1State == HIGH)) //if INPUT1 is pressed
    {
      currState = VEL3; //goes to a faster speed if input1 is pressed
      delay(del); //delay program for half a sec after state change
    }
    else if((oldButton2State == LOW) && (newButton2State == HIGH)) //if INPUT2 is pressed
    {
      prevState = VEL2; //saves current state so we can go back to it from the stop state
      currState = STOP; //goes to the stop state
      delay(del);
    }
    else
    {
      oldButton1State = newButton1State; //for debouncing again
      oldButton2State = newButton2State;
      myStepper.step(1); //steps once and loops. if we use steps per revolution, it will try to complete one full revolution before being able to read input
      
    }
  }
}

void stateVel3()
{
  currVelocity = currVelocity + 3; //third speed = 6rpm
  myStepper.setSpeed(currVelocity); //sets the new speed of the motor
  Serial.println("vel3"); //to explicitly tell which state we are in

  //as long as we don't press button1 or button2, we will remain in this state
  while(currState == VEL3) 
  {
    newButton1State = digitalRead(button1Pin); //read from pin of input1
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if((oldButton1State == LOW) && (newButton1State == HIGH)) //if INPUT1 is pressed
    {
      currState = VEL4; //if input1 is pressed, the program goes to a faster speed
      delay(del); //delay when changing states
    }
    else if((oldButton2State == LOW) && (newButton2State == HIGH)) //if INPUT2 is pressed
    {
      prevState = VEL3; //saves current state so we can go back from stop state
      currState = STOP; //goes to stop state
      delay(del); //delay when changing states
    }
    else
    {
      oldButton1State = newButton1State; //again, for debouncing
      oldButton2State = newButton2State;
      myStepper.step(1); //steps once and loops. if we use steps per revolution, it will try to complete one full revolution before being able to read input
      
    }
  }
}

void stateVel4()
{
  currVelocity = currVelocity + 3; //fourth and final speed = 12
  myStepper.setSpeed(currVelocity); //sets new speed of motor
  Serial.println("vel4"); //prints out which state we are in

  //will stay in state as long as we don't press button2. instructions don't say anything about cycling back to the first state with button1
  while(currState == VEL4)
  {
    newButton2State = digitalRead(button2Pin); //read from pin of input2
    if((oldButton2State == LOW) && (newButton2State == HIGH)) //if INPUT2 is pressed
    {
      prevState = VEL4; //saves current state so we can go back from stop state
      currState = STOP; //goes to stop state
      delay(del); //delay when changing states
    }
    else
    {
      oldButton2State = newButton2State; //for debouncing
      myStepper.step(1); //steps once and loops. if we use steps per revolution, it will try to complete one full revolution before being able to read input
    }
  }
}