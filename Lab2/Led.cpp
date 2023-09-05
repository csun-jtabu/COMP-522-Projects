/*
Jaztin Tabunda
Lab2
COMP 522 - V. Bhatt
4/13/2023
*/

//only writing some comments for this assignment (not my code), but code for lab2

#include "Led.h" //this file looks like it'll control the led
Led::Led(byte pin) {
  this->pin = pin;
  init();
}
void Led::init() {
  pinMode(pin, OUTPUT);
  off();
}
void Led::on() {
  digitalWrite(pin, HIGH);
}
void Led::off() {
  digitalWrite(pin, LOW);
}