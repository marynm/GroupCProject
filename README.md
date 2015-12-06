###Group C Project
# 
SYSC 3010 - Final Project
Group C

Party Bot

========================================
Files:
	Files for operation:
	controller.py
	dispener.py
	/src/iRobot.c

	Additional Files (testing etc.):
	controllerTest.py
	DispenserTest.py
	Bot.py
	audioTest.py

-----------------------------------------
Hardware Setup:

1. First Raspberry Pi is connected to PiFace.
2. Second Raspberry Pi is connected to stepper motor using GPIO pins. Stepper motor is attached to dispenser.
3. Third Raspberry Pi is connected to iRobot by USB, camera attached to Pi through USB, speaker attatched to Pi through audio port. Bowl, speaker, and camera are fixed to the top of the iRobot.

-----------------------------------------
Run Instructions:

Note: For set up connect each pi to a monitor. Once its code is running the monitor can be disconnected.

1. On Raspberry Pi 1 (controller) run controller.py. Once the code is running the monitor and other hardware (besides portable power and internet connections) can be removed and buttons used for operation (see below).

2. On Raspberry Pi 2 run dispenser.py.

3. On Raspberry Pi 3 run iRobot.c (made with MakeFile). Once running the monitor and other hardware must be disconnected (monitor, mouse, and keyboard) so that the bot can move freely.

-----------------------------------------
Operation:

Using the bottons on the controller the bot can be moved around the space and prompted to perform other actions.
	1. Button 1 is used to scroll thorugh the location selections (0-7), which are indicated by the lights on the controller.
	2. Button 2 confirms the location selection and sends the bot to that location. If the location is position (light) 7, the bot will go to the dispenser and be refilled
	3. Button 3 takes a picture.
	4. Button 4 changes the music.

