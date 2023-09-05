/*
Jaztin Tabunda
Traffic Controller/Lights with Semaphores
COMP 522 - V. Bhatt
4/15/2023
*/

#include <Arduino_FreeRTOS.h> //include this library for FreeRTOS
#include <semphr.h> //include this library to use Semaphores

TaskHandle_t taskHandler1; //task handler for the main program
TaskHandle_t taskHandler2; //task2 handler for the main program

TaskHandle_t interruptHandler; //task handler for the interrupt

SemaphoreHandle_t xBinarySemaphore; //create BinarySemaphore handler

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

volatile int mainStGreen = 5000; // 10 second delay
volatile int mainStYellow = 2000; // 5 second delay
volatile int sideStGreen = 5000; // 5 second delay
volatile int sideStYellow = 2000; //3 second delay

void task1_G1R2Y1R2(void *pvParameters);
void task2_R1G2R1Y2(void *pvParameters);

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
  
  xTaskCreate(task1_G1R2Y1R2, "MainGreen", 128, NULL,  1, &taskHandler1); //program starts by first putting the taskHandler
  xTaskCreate(task2_R1G2R1Y2, "MainRed", 128, NULL,  1, &taskHandler2); //program starts by first putting the taskHandler

  xBinarySemaphore = xSemaphoreCreateBinary(); // create Binary Semaphore
  xSemaphoreGive(xBinarySemaphore); //get it started by giving the binary semaphore to whichever task comes first or has highest priority
  
  vTaskStartScheduler(); //this is needed to start scheduling tasks whichever has the highest priority is done first
}

void loop() {
  //FreeRTOS handles this so no need for code here
}

//Main Green; Side Red
void stateG1R2()
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before 
  
  digitalWrite(greenPin1, HIGH); //turns on green1 and red2
  digitalWrite(redPin2, HIGH); 

  vTaskDelay( mainStGreen / portTICK_PERIOD_MS ); //keeps the light on for however long we want
}

//Main Yellow; Side Red
void stateY1R2()
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before
  
  digitalWrite(yellowPin1, HIGH); //turns on yellow1 and red2
  digitalWrite(redPin2, HIGH); 

  vTaskDelay( mainStYellow / portTICK_PERIOD_MS ); //keeps the light on for however long we want
}

//Main Red; Side Green
void stateR1G2()
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before
  
  digitalWrite(redPin1, HIGH); //turns on red1 and green2
  digitalWrite(greenPin2, HIGH);

  vTaskDelay( sideStGreen / portTICK_PERIOD_MS ); //keeps the light on for however long we want
}

//Main Red; Side Yellow
void stateR1Y2()
{
  turnOffLEDs(); //makes sure to turn off all previous LEDs before

  digitalWrite(redPin1, HIGH); //turns on red1 and yellow1
  digitalWrite(yellowPin2, HIGH);

  vTaskDelay( sideStYellow / portTICK_PERIOD_MS ); //keeps the light on for however long we want
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
        //Serial.println("button1 works"); 
        if( (digitalRead(greenPin2) == HIGH) && (digitalRead(redPin1) == HIGH) )
        {
            Serial.println("button1 when side is green works");
            vTaskDelete(taskHandler2); //deletes / resets task2
            xTaskCreate(interruptTask, "SwitchToMain", 128, NULL,  2, &interruptHandler); //creates interruptTask which will force yellow and switch right away
        } //if we want Main street to turn green
        else if( (digitalRead(greenPin1) == HIGH) && (digitalRead(redPin2) == HIGH) )
        {
            Serial.println("button1 when main is green works");
            vTaskDelete(taskHandler1); //deletes / resets task2
            vTaskDelete(taskHandler2); //deletes / resets task1
            xTaskCreate(task1_G1R2Y1R2, "MainGreen", 128, NULL,  1, &taskHandler1); //creates task1 before task2 so it can be scheduled first
            xTaskCreate(task2_R1G2R1Y2, "MainRed", 128, NULL,  1, &taskHandler2); 
        } //if we want Main street to remain green

        BaseType_t xHigherPriorityTaskWoken = pdFALSE; //basically interrupt is set to false first. it turns to true when it gets passed to the task with the highest priority
        xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken); //gives up semaphore to the next higher priority task
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
        //Serial.println("button2 works");
        if( (digitalRead(greenPin1) == HIGH) && (digitalRead(redPin2) == HIGH) )
        {
            Serial.println("button2 when main is green works");
            vTaskDelete(taskHandler1); //deletes / resets task1
            xTaskCreate(interruptTask2, "SwitchToSide", 128, NULL,  2, &interruptHandler); //creates interruptTask which will force yellow and switch to task2 
        }//if we want Side street to Turn green
        else if( (digitalRead(greenPin2) == HIGH) && (digitalRead(redPin1) == HIGH) )
        {
            Serial.println("button2 when side is green works");
            vTaskDelete(taskHandler2); //deletes / resets task2
            vTaskDelete(taskHandler1);//deletes / resets task1
            xTaskCreate(task2_R1G2R1Y2, "MainRed", 128, NULL,  1, &taskHandler2);  //creates task2 before task1 so it can be scheduled first
            xTaskCreate(task1_G1R2Y1R2, "MainGreen", 128, NULL,  1, &taskHandler1);
        } //if we want Side street to remain green

        BaseType_t xHigherPriorityTaskWoken = pdFALSE; //basically interrupt is set to false first. it turns to true when it gets passed to the task with the highest priority
        xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken); //gives up semaphore to the next higher priority task

      } //for debouncing
    } //for debouncing
  } //for debouncing
} //end of ISR2

//Regular cycle
void task1_G1R2Y1R2(void *pvParameters)
{
  while(1)
  {
    //makes sure the task1 priority is equal to task2's priority
    if(uxTaskPriorityGet(taskHandler1) != 1) 
    {
      vTaskPrioritySet(taskHandler1, 1);
    }
    xSemaphoreTake(xBinarySemaphore,portMAX_DELAY); //takes in semaphore so it can start the task
    stateG1R2(); //Main Green ; Side Red
    stateY1R2(); //Main Yellow ; Side Red
    xSemaphoreGive(xBinarySemaphore); //give up semaphore to next task
    vTaskDelay(1);
  }
}

//Regular cycle2
void task2_R1G2R1Y2(void *pvParameters)
{
  while(1)
  {
    //makes sure the task1 priority is equal to task1's priority
    if(uxTaskPriorityGet(taskHandler2) != 1)
    {
      vTaskPrioritySet(taskHandler2, 1);
    }    
    xSemaphoreTake(xBinarySemaphore,portMAX_DELAY); //takes in semaphore so it can start the task
    stateR1G2(); //Main Red ; Side Green
    stateR1Y2(); //Main Red ; Side Yellow
    xSemaphoreGive(xBinarySemaphore); //give up semaphore to next task
    vTaskDelay(1);
  }
}

void interruptTask(void *pvParameters)
{
  if(xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdPASS) //makes sure that semaphore is passed to it
  {
    stateR1Y2(); //Main Red ; Side Yellow
    xSemaphoreGive(xBinarySemaphore); //give up semaphore to next task. we give up the semaphore first so it's forced to schedule task1 first before current task deletes itself and creates task2
    xTaskCreate(task2_R1G2R1Y2, "MainRed", 128, NULL,  1, &taskHandler2); //creates task2
    vTaskDelete(interruptHandler); //task deletes itself
    vTaskDelay(1);
  } 
}

void interruptTask2(void *pvParameters)
{
  if(xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdPASS)
  {
    stateY1R2();
    xSemaphoreGive(xBinarySemaphore); //give up semaphore to next task. we give up the semaphore first so it's forced to schedule task2 first before current task deletes itself and creates task1
    xTaskCreate(task1_G1R2Y1R2, "MainGreen", 128, NULL,  1, &taskHandler1); //creates task1
    vTaskDelete(interruptHandler); //task deletes itself
    vTaskDelay(1);
  } 
}