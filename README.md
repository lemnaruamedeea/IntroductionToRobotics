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

## Homework 3 - 7 segment display drawing
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

## Homework 4 - Stopwatch timer
I'm tasked with creating a stopwatch timer using a 4-digit 7-segment display and 3 buttons. The timer should count in 10ths of a second and have a lap-saving feature similar to basic stopwatch functions found on most phones.
My display begins with "000.0". When I press the Start button, the timer kicks into action.
While the timer runs, each time I press the lap button, the current timer value should be saved temporarily in memory, up to 4 laps or more if needed. If I press the lap button for the 5th time, it will override the first saved value. If I hit the reset button during timing, nothing changes. However, if I hit pause, the timer halts.
When in Pause Mode, the lap button won't save any further laps. If I press reset, it resets the timer display back to "000.0".
After a reset, I tryed to use the lap buttons to cycle through the saved lap times. Every press should have taken me to the next saved lap. Continuously pressing the lap button should have cycled through all the saved laps, but I didn't succeded practically, but in theory, it works.
Components used:

-1  4-digit 7-segment  display, 

-3  buttons, 

-8 resistors,

-lots of wires

![SetupImg](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework4/SetupImg.jpeg?raw=true)

https://youtube.com/shorts/Ss_1BUEqMBo?si=jCl7402cfeI99Ji6
