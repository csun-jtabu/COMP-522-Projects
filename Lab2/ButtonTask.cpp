/*
Jaztin Tabunda
Lab2
COMP 522 - V. Bhatt
4/13/2023
*/

//only writing some comments for this assignment (not my code), but code for lab2

#include "ButtonTask.h"

bool ButtonTask::run() { //this is the task that runs the button
  button_semaphore.wait();
  Log.traceln("Button pushed");
  msleep(100); // Debounce
  button_semaphore.wait(0); // Clear possible outstanding semaphore posts

  if (isAwake) {
    isrs::disableSoundSensorISR(); // We don't want the buzzer to start sound interrupt
    tone(BUZZER_PIN, buzzerOffTone, buzzerDuration);
    msleep(buzzerDuration); // Wait for tone to complete before going to power save mode
    isrs::enableSoundSensorISR(); // Re-enable
    isAwake = false;
    digitalWrite(INDUCTION_LEDS_PIN, LOW);
  } else {
    isAwake = true;
    Log.traceln("Wake up");
    isrs::disableSoundSensorISR(); // We don't want the buzzer to start sound interrupt
    tone(BUZZER_PIN, buzzerOnTone, buzzerDuration);
    isrs::enableSoundSensorISR(); // Re-enable
    wakeup_semaphore.post();
  }
  return true;
}
