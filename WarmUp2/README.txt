Jaztin Tabunda
Warm-Up Project 2 - Step Motor with Button Control
COMP 522 - V. Bhatt
3-3-2023

Besides Research on this Project, Steps I took:

1. Connected Arduino UNO to Laptop. This allowed power to flow from my laptop to the 
Arduino board
2. Plugged Buttons (INPUT1 and INPUT2) onto BreadBoard
3. Connected Red jumper cables from positive terminal to one leg of each of the buttons 
	and connected 10k Ohm resistors to negative terminal.
4. Connected Blue jumper cables from to each input buttons' legs to pins 2 and 3 respectively
5. Plugged Power Supply Module onto Breadboard's positive terminals and negative terminals 
6. Connected 9 V power adapter to Power Supply Module
7. Connected cables from Step Motor Driver's positive and negative terminals to their
	respective terminals on the breadboard
8. Connected several more cables going from the Step Motor Driver's IN1, IN2, IN3, IN4
	prongs to Arduino's pins 8, 9, 10, and 11
9. Connected the Step Motor to the Step Motor Driver
10. Turned on Power Supply Module
11. With everything connected, I tested the connections with a simple .step() function
12. Step Motor works so I started to program the state machine
13. Wrote the program, compiled, and tested according to the specifications

How much of the Project is working:
Project is Finished. The Program can do what specifications asked. 
In other words... 
- Program increases stepper motor speed with a button (INPUT1)
- When running, program stops stepper motor with another button (INPUT2)
- From Stop state, can go back to previously selected velocity with INPUT1
- From Stop state, can reset current velocity back to 0 with INPUT2