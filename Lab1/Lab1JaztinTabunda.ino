/*
Jaztin Tabunda
Lab 1 - using FreeRTOS on Arduino -Will do today in class
COMP 522 - V. Bhatt
4-6-2023
*/


#include <Arduino_FreeRTOS.h> //this line of code includes the FreeRTOS Library

void TaskBlink1( void *pvParameters ); //this line of code defines the method and the parameters of TaskBlink1. the parameter points to the address location of parameter being passed

void TaskBlink2( void *pvParameters ); //this line of code defines the method and the parameters of TaskBlink2. the parameter points to the address location of parameter being passed

void Taskprint( void *pvParameters ); //this line of code defines the method and the parameters of Taskprint. the parameter points to the address location of parameter being passed

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  //this is how you create a task.
  //Essentially (name of function, string name of task, number of words stack can hold, any parameters that are being passed into method, 
  //priority being served, can be used for handler of task)
  //in these 3 cases, the number of words is 128
  xTaskCreate(TaskBlink1, "task1", 128, NULL, 0, NULL);  
  xTaskCreate(TaskBlink2, "task2", 128, NULL,  1, NULL); 
  xTaskCreate(Taskprint, "task3", 128, NULL, 2, NULL); 
  vTaskStartScheduler(); //this is needed to start scheduling tasks whichever has the highest priority is done first
}

void loop()
{}

void TaskBlink1(void *pvParameters)  
{
  pinMode(8, OUTPUT); //sets up pin 8 as output
  while(1) //infinite while loop until task scheduler transfers control to another task
  {
    Serial.println("Task1"); 
    digitalWrite(8, HIGH);   //turns on led
    vTaskDelay( 500 / portTICK_PERIOD_MS ); //special delay when using FreeRTOS 
    digitalWrite(8, LOW);    //turns off led
    vTaskDelay( 500 / portTICK_PERIOD_MS ); //special delay when using FreeRTOS
  }
}

void TaskBlink2(void *pvParameters)  
{
  pinMode(7, OUTPUT); //sets up pin 7 as output
  while(1) //infinite while loop until task scheduler transfers control to another task
  {
    Serial.println("Task2");
    digitalWrite(7, HIGH);   //turns on led
    vTaskDelay( 400 / portTICK_PERIOD_MS ); //special delay when using FreeRTOS
    digitalWrite(7, LOW);   //turns off led
    vTaskDelay( 400 / portTICK_PERIOD_MS ); //special delay when using FreeRTOS
  }

}

void Taskprint(void *pvParameters)  {

  int counter = 0; //counts how many times the this task is ran
  while(1) //infinite while loop until task scheduler transfers control to another task
  {
    counter++; //increments count everytime this task is ran
    Serial.println(counter); 
    vTaskDelay(500 / portTICK_PERIOD_MS);    //special delay for FreeRTOS
  }
}