# IntroductionToRobotics
This repository is meant to illustrate my jurney through the "Introduction To Robotics" course taken in my third year at the Faculty of Mathematics and Computer Science, University of Bucharest. In the end, I hope I'll be able to show my progress through my projects and laboratory homeworks, which will include photos, code and documentation.

<details>
<summary>< ### Homework 1 - RGB LED</summary>

This task is centered around the manipulation of each channel (Red, Green, and Blue) of an RGB LED through the use of separate potentiometers. Upon completion of this assignment, I have acquired skills in connecting potentiometers to an Arduino and manipulating RGB LEDs based on analog input values.

![SetupImg1](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework1/SetupImg1.jpeg?raw=true)
![SetupImg2](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework1/SetupImg2.jpeg?raw=true)

https://youtu.be/iNuT3lebwAA?si=6-tmb08IwD-qQzN6

</details>


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

## Homework 5 - ”Smart Environment Monitor and Logger”
I will create a "Smart Environment Monitor and Logger" with Arduino. I'll use different sensors to collect environmental information, save it into EEPROM memory, and offer visual feedback using an RGB LED. Additionally, I'll enable user interaction through a Serial Menu. My main goal is to integrate sensor data, manage memory, establish Serial Communication, and ultimately create a functional menu system.

Components used:
-Arduino Uno Board

-Ultrasonic Sensor (HC-SR04)

-LDR (Light-Dependent Resistor) aka Photocell aka Photoresistor aka LightSensor

-RGB LED

-4 Resistors

-Breadboard

-connecting wires (lots of them)

Menu Structure:
1. Sensor Settings
  1.1 Set Sensors Sampling Interval: You'll choose a value between 1 and 10 seconds, which will determine the sampling rate for the sensors.
  1.2 Set Ultrasonic Alert Threshold: You'll choose a threshold value for the ultrasonic sensor, indicating a boundary (min or max) value. If the sensor value surpasses this threshold, an alert message will trigger. If the LED is in Automatic Mode (see section 4.2), it will turn red when any sensor value exceeds this set boundary.
  1.3 Set LDR Alert Threshold: You'll choose a threshold value for the LDR sensor, indicating a boundary (min or max) value. When the sensor value crosses this threshold, an alert message will activate. Similar to the Ultrasonic sensor, if the LED is in Automatic Mode (see section 4.2), it will turn red when any sensor value goes beyond this set boundary.
  1.4 Back: Return to the main menu
2. Resetting Logger Data: When selected, it will print a message asking for confirmation to delete all data, something like "Are you sure?" Following this, a submenu will appear offering options for YES or NO to confirm the reset for both sensors.
  2.1 Yes.
  2.2 No.
3. System Status: I created a system status section with several options to provide relevant information about the system's functionality and data.
  3.1 Current Sensor Readings: This option will continuously display sensor readings at the set sampling rate from all sensors. I ensured there's a way to exit this display, by pressing the "x" key. I also included a message to inform users of this exit method.
  3.2 Current Sensor Settings: This will display the sampling rate and threshold values for all sensors.
  3.3 Display Logged Data: This option will show the last 10 sensor readings for all sensors.
  3.4 Back: This choice will allow users to return to the Main menu.
4. RGB LED Control:
  4.1 Manual Color Control: This option will allow manual setting of the RGB colors for the LED. I defined the input method as a number beteew 0 and 255. I informed the user about the specific format required.
  4.2 LED: Toggle Automatic ON/OFF: When Automatic Mode is ON, the LED will display GREEN when all sensor values are within their threshold limits (no alert) and RED when any sensor value exceeds its threshold (indicating an alert). If Automatic Mode is OFF, the LED will use the last saved RGB values.
  4.3 Back: This option will enable users to return to the Main menu.

![SetupImg1](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework5/SetupImg1.jpeg?raw=true)
![SetupImg2](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework5/SetupImg2.jpeg?raw=true)
![SetupScheme](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework5/SetupScheme.png?raw=true)

https://youtu.be/1lsIuqp7mLA?feature=shared

## Homework 6 - 8x8 matrix game
I created a small game using an 8x8 matrix. In this game, I incorporated three types of elements: a player (blinking slowly), bombs/bullets (blinking rapidly), and walls (no blinking). The goal is to kickstart my matrix project. The main concept involves generating walls across the map (between 50% to 75% coverage) and maneuvering the player to destroy them. I approached it akin to Bomberman.

Components used:
-Arduino Uno Board

-Joystick

-8x8 LED Matrix

-MAX7219

-1 Resistor

-2 Capacitors

-1 Mini-Breadboard

-1 Mid-Breadboard

- Looots of connecting wires

Things I took care of:
1. LED Differentiation: I ensured that the player and bombs/bullets blink at distinct rates. Specifically, the player blinks slowly, while bombs/bullets blink rapidly. Additionally, the walls remain unblinking.

2. Control: Smooth control is crucial, so I made the gameplay experience enjoyable and user-friendly.

3. Control: I prevented the initial generation of walls on top of the player when the game begins. I ensured that the player isn't obstructed by walls from the start.

![SetupImg1](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework6/SetupImg1.jpeg?raw=true)
![SetupImg2](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework6/SetupImg2.jpeg?raw=true)

https://youtu.be/tqwe8zg4jlE?si=lTHsavzCXjQEaUq0

## Homework 7 - 8x8 matrix game LCD menu

When I start the project, it displays a greeting message briefly. The menu options are:

1. Starting the game, which initiates the first level.
   
2. Settings:
(a) Control LCD brightness, saving the value to EEPROM and loading it on system start.
(b) Manage Matrix brightness using the 'set Intensity' function from the led-Control library. Display something on the matrix when selected. Save the value to EEPROM and load it on system start.
(c) About: This section includes details about me, the game's author and my GitHub username (using scrolling text).

3. When the game concludes, it will show a message and wait for a user prompt, wich is a button push, before returning to the main menu.

4.During gameplay, it will display dynamic details, more specific, lives.

5. To wrap up a game, I implemented an end-game functionality. The game ends when all the "walls" are eliminated. Afterward, it will display another message, wait for a user prompt, and return to the main menu.

Components used:
-Arduino Uno Board

-Joystick

-8x8 LED Matrix

-LCD Display

-MAX7219

-1 Resistor

-2 Capacitors

-1 Mini-Breadboard

-1 Mid-Breadboard

-Looots of connecting wires

![SetupImg1](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework7/SetupImg1.jpeg?raw=true)
![SetupImg2](https://github.com/lemnaruamedeea/IntroductionToRobotics/blob/main/Homework7/SetupImg2.jpeg?raw=true)

https://youtu.be/TS1xVqFHN4I?si=d14isFyX6EQPvKsz
