# IntroductionToRobotics
This repository is meant to illustrate my jurney through the "Introduction To Robotics" course taken in my third year at the Faculty of Mathematics and Computer Science, University of Bucharest. In the end, I hope I'll be able to show my progress through my projects and laboratory homeworks, which will include photos, code and documentation.

## Homework 1 - RGB LED
This task is centered around the manipulation of each channel (Red, Green, and Blue) of an RGB LED through the use of separate potentiometers. Upon completion of this assignment, I have acquired skills in connecting potentiometers to an Arduino and manipulating RGB LEDs based on analog input values.

![SetupImg1](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework1/SetupImg1.jpeg?raw=true)
![SetupImg2](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework1/SetupImg2.jpeg?raw=true)

https://youtu.be/iNuT3lebwAA?si=6-tmb08IwD-qQzN6

## Homework 2 - Elevator Simulator Wannabe
Creating a control system to replicate the operations of a three-floor elevator using the Arduino platform presented an intriguing challenge. Meeting the specific requirements while making it all come together was a fascinating journey into the world of electronics and programming.
My design included three LEDs, each representing a floor, with the LED corresponding to the elevator's current position illuminating. An additional LED indicated the elevator's operational state, blinking during motion and remaining steady when stationary.
Three buttons, resembling call buttons on each floor, triggered the elevator's simulated movement.
To enhance the simulation, a buzzer emitted distinct sounds for the elevator's arrival and door operations, replicating real-world experiences.
To optimize user interaction, the system was programmed to ignore redundant button presses when the elevator was already at the desired floor.
Debounce mechanisms for the buttons ensured a smooth and reliable user experience.
In summary, the design and implementation of this control system for a simulated three-floor elevator using the Arduino platform was both a challenging and fulfilling endeavor.

![SetupImg1](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework2/SetupImg1.jpeg?raw=true)
![SetupImg2](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework2/SetupImg2.jpeg?raw=true)
![SetupScheme](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework2/SetupScheme.png?raw=true)

https://youtu.be/0VN2qUHWAPw?si=XpcYRdrIy6sqkWz8

## Homework 2 - 7 segment display drawing
I used the joystick to control the position of the segment and "draw" on the display. I wanted the movement between segments to feel natural, so they should transition smoothly from the current position to neighboring positions without passing through any "walls."
The initial position is on the DP. The current position will always blink, regardless of whether the segment is on or off. I use the joystick to move from one position to neighboring ones (as indicated in the table for corresponding movements). A short press of the button will toggle the segment state between ON and OFF. If I long-press the button, it will reset the entire display by turning off all the segments and moving the current position to the decimal point.

![Table of corresponding movements](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework3/Table_of_corresponding_movements.png?raw=true)

Components used:

-1  7-segment  display, 
-1  joystick, 
-8 resistors,
-lots of wires

![SetupImg1](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework3/SetupImg1.jpeg?raw=true)

![SetupImg2](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework3/SetupImg2.jpeg?raw=true)

https://youtube.com/shorts/5fE8dwfEra8
