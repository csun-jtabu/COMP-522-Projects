/*
Jaztin Tabunda
Lab2
COMP 522 - V. Bhatt
4/13/2023
*/

//only writing some comments for this assignment (not my code), but code for lab2

#include "ClapTask.h"

bool ClapTask::run() { //this file looks like it runs when it detects sound
  clap_semaphore.wait();
  Log.traceln("Sound detected"); 
  msleep(400); // Debounce
  clap_semaphore.wait(0); // Clear possible outstanding semaphore posts

  if (isInductionLedsOn) {
    isInductionLedsOn = false;
    inductionLeds.off();
  } else {
    isInductionLedsOn = true;
    inductionLeds.on();
  }

  servo_semaphore.post();
  return true;
}
