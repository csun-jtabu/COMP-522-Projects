/*
Jaztin Tabunda
Traffic Controller
COMP 522 - V. Bhatt
4/4/2023
*/

int redPin1 = 3; //pin numbers for first set of traffic light
int yellowPin1 = 2;
int greenPin1 = 1;

int redPin2 = 10; //pin numbers for second set of traffic light
int yellowPin2 = 9;
int greenPin2 = 8;

int del = 5000; // 5 second delay for alternating states

enum States{G1R2, Y1R2, R1G2, R1Y2}; //state name GreenLight1RedLight2, YellowLight1RedLight2, etc.
States currentState = G1R2; //current state

void setup() {
  // put your setup code here, to run once:
  pinMode(redPin1, OUTPUT); //sets up traffic light set 1 as output
  pinMode(yellowPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);
  
  pinMode(redPin2, OUTPUT); //sets up traffic light set 1 as output
  pinMode(yellowPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //traffic control states
  switch(currentState)
  {
    case G1R2:
    stateG1R2(); 
    break;

    case Y1R2:
    stateY1R2();
    break;

    case R1G2:
    stateR1G2();
    break;

    case R1Y2:
    stateR1Y2();
    break;
  }
}

void stateG1R2()
{
  digitalWrite(greenPin1, HIGH); //turns on green1 and red2
  digitalWrite(redPin2, HIGH); 
  delay(del); //keeps the light on for however long we want
  digitalWrite(greenPin1, LOW); //turns off green1 and red2
  digitalWrite(redPin2, LOW);
  stateY1R2(); //switches states to change leds
}

void stateY1R2()
{
  digitalWrite(yellowPin1, HIGH); //turns on yellow1 and red2
  digitalWrite(redPin2, HIGH); 
  delay(del); //keeps the light on for however long we want
  digitalWrite(yellowPin1, LOW); //turns off yellow1 and red2
  digitalWrite(redPin2, LOW);
  stateR1G2(); //switches states to change leds
}

void stateR1G2()
{
  digitalWrite(redPin1, HIGH); //turns on red1 and green2
  digitalWrite(greenPin2, HIGH);
  delay(del); //keeps the light on for however long we want
  digitalWrite(redPin1, LOW); //turns off red1 and green2
  digitalWrite(greenPin2, LOW);
  stateR1Y2(); //switches states to change leds
}

void stateR1Y2()
{
  digitalWrite(redPin1, HIGH); //turns on red1 and yellow1
  digitalWrite(yellowPin2, HIGH);
  delay(del); //keeps the light on for however long we want
  digitalWrite(redPin1, LOW); //turns off red1 and yellow1
  digitalWrite(yellowPin2, LOW);
  stateG1R2(); //switches states to change leds
}