/*
Jaztin Tabunda
FreeRTOS Traffic Controller
COMP 522 - V. Bhatt
4/11/2023
*/

#include <Arduino_FreeRTOS.h> //this line of code includes the FreeRTOS Library

TaskHandle_t Th1; //task handler for the main program

int redPin1 = 6; //pin numbers for Main Street Traffic Lights
int yellowPin1 = 5;
int greenPin1 = 4;

int redPin2 = 13; //pin numbers for Side Street Traffic Lights
int yellowPin2 = 12;
int greenPin2 = 11;

int buttonPin = 2; //button to simulate car on Main Street (button1)
int buttonPin2 = 3; //button sto simulate car on side street (button2)

int oldButtonState1 = 0; //for debouncing
int newButtonState1 = 0;
int oldButtonState2 = 0;
int newButtonState2 = 0;

int lastButtonChange = millis(); //for debouncing
int lastButtonChange2 = millis();

int debounce = 50; //debounce time

int mainStGreen = 10000; // 10 second delay
int mainStYellow = 5000; // 5 second delay
int sideStGreen = 5000; // 5 second delay
int sideStYellow = 3000; //3 second delay

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Serial.begin gets the arduino ready to communicate with text output

  pinMode(buttonPin, INPUT); // sets up pin 2 as input for button1
  pinMode(buttonPin2, INPUT); // sets up pin 3 as input for button2
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISRButton1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), ISRButton2, CHANGE);

  pinMode(redPin1, OUTPUT); //sets up traffic light set 1 as output
  pinMode(yellowPin1, OUTPUT);
  pinMode(greenPin1, OUTPUT);

  pinMode(redPin2, OUTPUT); //sets up traffic light set 1 as output
  pinMode(yellowPin2, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  
  xTaskCreate(stateG1R2, "RegularInterval", 128, NULL,  0, &Th1); //program starts by first putting the taskHandler
  
  vTaskStartScheduler(); //this is needed to start scheduling tasks whichever has the highest priority is done first
}

void loop() {
  //FreeRTOS handles this so no need for code here
}

//Main Green; Side Red
void stateG1R2(void *pvParameters)
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before 
  
  digitalWrite(greenPin1, HIGH); //turns on green1 and red2
  digitalWrite(redPin2, HIGH); 

  vTaskDelay( mainStGreen / portTICK_PERIOD_MS ); //keeps the light on for however long we want
  xTaskCreate(stateY1R2, "RegularInterval", 128, NULL,  0, &Th1); //this creates and goes to the next task
  vTaskDelete(NULL); //terminates this current method/task
}

//Main Yellow; Side Red
void stateY1R2(void *pvParameters)
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before
  
  digitalWrite(yellowPin1, HIGH); //turns on yellow1 and red2
  digitalWrite(redPin2, HIGH); 

  vTaskDelay( mainStYellow / portTICK_PERIOD_MS ); //keeps the light on for however long we want
  xTaskCreate(stateR1G2, "RegularInterval", 128, NULL,  0, &Th1); //this creates and goes to the next task
  vTaskDelete(NULL); //terminates this current method/task
}

//Main Red; Side Green
void stateR1G2(void *pvParameters)
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before
  
  digitalWrite(redPin1, HIGH); //turns on red1 and green2
  digitalWrite(greenPin2, HIGH);

  vTaskDelay( sideStGreen / portTICK_PERIOD_MS ); //keeps the light on for however long we want
  xTaskCreate(stateR1Y2, "RegularInterval", 128, NULL,  0, &Th1); //this creates and goes to the next task
  vTaskDelete(NULL); //terminates this current method/task
}

//Main Red; Side Yellow
void stateR1Y2(void *pvParameters)
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before

  digitalWrite(redPin1, HIGH); //turns on red1 and yellow1
  digitalWrite(yellowPin2, HIGH);

  vTaskDelay( sideStYellow / portTICK_PERIOD_MS ); //keeps the light on for however long we want
  xTaskCreate(stateG1R2, "RegularInterval", 128, NULL,  0, &Th1); //this creates and goes to the next task
  vTaskDelete(NULL); //terminates this current method/task
}

//turns off all LEDs
void turnOffLEDs()
{
  digitalWrite(redPin1, LOW); //trafficLight1
  digitalWrite(yellowPin1, LOW);
  digitalWrite(greenPin1, LOW);

  digitalWrite(redPin2, LOW); //trafficLight2
  digitalWrite(yellowPin2, LOW);
  digitalWrite(greenPin2, LOW);
}

//if there was a car on Main St
void ISRButton1()
{
  if(millis() - lastButtonChange >= debounce)
  {
    newButtonState1 = digitalRead(buttonPin);
    if(newButtonState1 != oldButtonState1)
    {
      lastButtonChange = millis();
      oldButtonState1 = newButtonState1;
      if(oldButtonState1 == LOW)
      {
        Serial.println("button1 works"); 
        if( (digitalRead(greenPin2) == HIGH) && (digitalRead(redPin1) == HIGH) )
        {
          if(Th1 != NULL)
          {
            if(uxTaskPriorityGet(Th1) <= 3) 
            {
              vTaskDelete(Th1); //frees up the taskhandler
              xTaskCreate(stateR1Y2, "ForceMain", 128, NULL,  3, &Th1);
              //notice priority = 3 because it's the most important
            } //if the current task's priority is lower than what we'll replace it with
          } //redundant, but makes sure the taskhandler isn't null
        } //if we want Main street to turn green
        else if( (digitalRead(greenPin1) == HIGH) && (digitalRead(redPin2) == HIGH) )
        {
          if(Th1 != NULL)
          {
            if(uxTaskPriorityGet(Th1) <= 3)
            {
              vTaskDelete(Th1); //frees up the taskhandler
              xTaskCreate(stateG1R2, "StayMain", 128, NULL,  3, &Th1); 
              //notice priority = 3 because it's the most important   
            } //if the current task's priority is lower than what we'll replace it with
          } //redundant, but makes sure the taskhandler isn't null
        } //if we want Main street to remain green
      } //for debouncing
    } //for debouncing
  } //for debouncing
} //end of ISR2

//if there was a car on Side st
void ISRButton2()
{
  if(millis() - lastButtonChange2 >= debounce)
  {
    newButtonState2 = digitalRead(buttonPin2);
    if(newButtonState2 != oldButtonState2)
    {
      lastButtonChange2 = millis();
      oldButtonState2 = newButtonState2;
      if(oldButtonState2 == LOW)
      {
        Serial.println("button2 works");
        if( (digitalRead(greenPin1) == HIGH) && (digitalRead(redPin2) == HIGH) )
        {
          if(Th1 != NULL)
          {
            if(uxTaskPriorityGet(Th1) <= 2)
            {
              vTaskDelete(Th1);
              xTaskCreate(stateY1R2, "ForceSide", 128, NULL,  2, &Th1); 
              //notice priority = 2 because it's more important than the regular light program, but less important than the Main street interrupt  
            } //if the current task's priority is lower than what we'll replace it with
          } //redundant, but makes sure the taskhandler isn't null
        }//if we want Side street to Turn green
        else if( (digitalRead(greenPin2) == HIGH) && (digitalRead(redPin1) == HIGH) )
        {
          if(Th1 != NULL)
          {
            if(uxTaskPriorityGet(Th1) <= 2)
            {
              vTaskDelete(Th1);
              xTaskCreate(stateR1G2, "StaySide", 128, NULL,  2, &Th1); 
            } //if the current task's priority is lower than what we'll replace it with
          } //redundant, but makes sure the taskhandler isn't null
        } //if we want Side street to remain green
      } //for debouncing
    } //for debouncing
  } //for debouncing
} //end of ISR2