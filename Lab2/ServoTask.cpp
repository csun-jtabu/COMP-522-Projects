/*
Jaztin Tabunda
Lab2
COMP 522 - V. Bhatt
4/13/2023
*/

//only writing some comments for this assignment (not my code), but code for lab2

#include "ServoTask.h"

ServoTask::ServoTask() {
}

bool ServoTask::run() { //this task/file looks to be what controls the servo
  int valueDegrees = 0;
  servo_semaphore.wait();
  isrs::disableSoundSensorISR(); // We don't want the servo to start sound interrupt
  if (isAwake) {
    Log.traceln("Move the servo");
    if (isInductionLedsOn) {
      servo.write(180);
    } else {
      servo.write(0);
    }
  }
  msleep(1000);
  isrs::enableSoundSensorISR();
  return true;
}
