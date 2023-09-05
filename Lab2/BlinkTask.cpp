/*
Jaztin Tabunda
Lab2
COMP 522 - V. Bhatt
4/13/2023
*/

//only writing comments for this assignment (not my code), but code for lab2

//this file looks like is for the Blinking Task for LED and is written in c++

#include "BlinkTask.h"

bool BlinkTask::run() {
  if (isAwake) { //this code only runs when the program is on
    Log.traceln("Blink");
    builtInLed.on(); //turns led on
    msleep(onPeriod); //essentially delays the led from turning off to make it blink
    builtInLed.off(); //turns led off
    msleep(offPeriod); //delays again
  } else {
    builtInLed.off(); //turns led off
    Log.traceln("Go to sleep");  //basically stays off until toggle again
    wakeup_semaphore.wait(); // When the awake toggle is off, go to sleep
  }
  yield();
  return true;
}