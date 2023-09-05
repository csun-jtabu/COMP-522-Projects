Jaztin Tabunda
Traffic Signal Lights Semaphores
COMP 522 - V. Bhatt
4-17-2023

This program is a traffic light controller with FreeRTOS AND SEMAPHORES

This program does the following:
1. Cycles between a Main Street and Side Street
The Intervals for each light is as follows:
______________________________________________
Main Green = 10 sec	Side Red = 15 sec
----------------------------------------------
Main Yellow = 5 sec	Side Yellow = 3 sec
----------------------------------------------
Main Red = 8 sec	Side Green = 5 sec
______________________________________________

2. With the buttons you can force a green on each light
- Button1 forces 'Side Green' to end prematurely and starts 'Side Yellow'. 
Additionally, it can lengthen the time for 'Main Green'
- Button2 forces 'Main Green' to end prematurely and starts 'Main Yellow'.
Additionally, it can lengthen the time for 'Side Green'

Although you can interrupt both lights, 'Main Street' (Button1) has a higher priority
than 'Side Street' (Button2). Therefore, if both buttons are pressed, Main Street will
be done and Side Street will be ignored.

The difference between this and the other submissions is that this uses FreeRTOS and Semaphores