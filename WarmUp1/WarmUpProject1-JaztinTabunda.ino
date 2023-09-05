/*
Jaztin Tabunda
Warm-Up Project 1 - Potentiometer
COMP 522 - V. Bhatt
2-5-2023
*/

const int ledPin = 6; //This is the pin number for the led (output)
const int inputPin = A0; //This is the pin number for the potentiometer (input)
int inputSignal = 0; //This is to store the 0-1023 value that the potentiometer will output
float outputSignal = 0; //This is to store the converted 0-255 value from the inputSignal value

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT); //sets up pin 6 as Output
  pinMode(inputPin, INPUT); //sets up pin A0 as Input
  Serial.begin(9600); //This is the speed of the constant stream of data that will be taken from the inputPin/Potentiometer
}

void loop() {
  // put your main code here, to run repeatedly:
  inputSignal = analogRead(inputPin); //getting the input from the inputPin and storing it in inputSignal
  outputSignal = map(inputSignal, 0, 1023, 0, 255); //mapping/converting the 0-1023 value of the potentiometer to the PWM 0-255 output signal 
  analogWrite(ledPin, outputSignal); //this will overwrite the value that will be outputted to the ledPin
  //Serial.println(outputSignal); //this is used to check if the potentiometer is working 
  delay(500); //this delay is used so the input is more stable

}
