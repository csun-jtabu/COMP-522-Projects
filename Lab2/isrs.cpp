/*
Jaztin Tabunda
Lab2
COMP 522 - V. Bhatt
4/13/2023
*/

//only writing some comments for this assignment (not my code), but code for lab2

#include "isrs.h"

void isrs::buttonISR() { //this file looks to be the interrupts for each device
  button_semaphore.preparePostFromInterrupt();
  button_semaphore.postFromInterrupt();
  button_semaphore.finalizePostFromInterrupt();
}

void isrs::soundSensorISR() {
  
  clap_semaphore.preparePostFromInterrupt();
  clap_semaphore.postFromInterrupt();
  clap_semaphore.finalizePostFromInterrupt();
}

void isrs::enableButtonISR() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), isrs::buttonISR, LOW);
}

void isrs::enableSoundSensorISR() {
  pinMode(SOUND_SENSOR_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SOUND_SENSOR_PIN), isrs::soundSensorISR, HIGH);
}

void isrs::disableSoundSensorISR() {
  detachInterrupt(digitalPinToInterrupt(SOUND_SENSOR_PIN));
}

// Timer interrupt, used to refresh servo every 20ms
volatile int counter = 0;
ISR(TIMER0_COMPB_vect) {
  // this gets called every millisecond
  counter++;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
    counter = 0;
    if (isAwake) servo.refresh();
  }
}