#The controller pi, used as the 'remote' to direct the bot actions (moving, food fill up, music control)

import pifacedigitalio as p
from time import sleep

#initial basic functionality:
#when the buttons 'left' or 'right' are pressed the lights move to indicate 
#which of the 8 possible pre-programmed loactions the bot will move to
#(later this will also send a message to the bot telling it to go to that position)

p.init()


def button_monitor():
	position = 0	#keeps track of the current position of the bot
	x = 0

	while(1):
		while(x!=1):
			if(p.digital_read(0)):			#button 0 pressed: move to next postion 'up'
				sleep(0.5)
				position = position + 1
				if(position == 8):
					position = 0		#roll-over
				position_lights(position)	#update the lights

			elif(p.digital_read(1)):		#button 1 pressed: confirm current selection
				sleep(0.5)
				x = 1	
				move_bot(position)		#tell bot to move

				if(position == 8):		#if the bot is going to the dispenser, alert the despiensor to activate the sensors
					alert_dispenser();

				#disactivate buttons 0 and 1 until bot has reached location	
		
			elif(p.digital_read(2)):		#button 2 pressed: Take picture
				sleep(0.5)
				flash_lights()			#flash lights to indicate a button press
				position_lights(position)	#then return to indicating the position
				take_picture()
		
			elif(p.digital_read(3)):		#button 3 pressed: Change song
				sleep(0.5)
				flash_lights()			#flash lights to indicate a button press
				position_lights(position)	#then return to indicating the position
				change_song()

		#while the bot is moving buttons 0 and 1 are disactivated until the bot reaches the location
		while(x ==1):
			
			if(p.digital_read(2)):		#button 2 pressed: Take picture
				sleep(0.5)
				flash_lights()			#flash lights to indicate a button press
				position_lights(position)	#then return to indicating the position
				take_picture()
		
			elif(p.digital_read(3)):		#button 3 pressed: Change song
				sleep(0.5)
				flash_lights()			#flash lights to indicate a button press
				position_lights(position)	#then return to indicating the position
				change_song()
			if(msg_from_bot() == 1)			#check for message from bot saying that the bot has reached location
				x = 0

	
	


def move_bot(position):
#send message to bot telling it which position to go to

	if(position != 7):
		print("Moving bot to location " + str(position))
	else:
		print("Moving bot to dispenser");

	return position

def take_picture():
#send message to bot telling it to take a picture
	print("Taking picture");

def change_song():
#send message to bot telling it to change the song
	print("Changing song");

def alert_dispenser():
#send message to the bot telling it that the bot is coming, so that it activates the sensors and is prepaered to dispense
	print("Alerting dispenser")

def int msg_from_bot()
	#check for message from bot

	#if msg received, return 1
		return 1

def flash_lights():
#flash all of the lights on and off once
	p.digital_write(0,1)
	p.digital_write(0,1)
	p.digital_write(1,1)
	p.digital_write(2,1)
	p.digital_write(3,1)
	p.digital_write(4,1)
	p.digital_write(5,1)
	p.digital_write(6,1)
	p.digital_write(7,1)

	sleep(0.2)

	p.digital_write(0,0)
	p.digital_write(1,0)
	p.digital_write(2,0)
	p.digital_write(3,0)
	p.digital_write(4,0)
	p.digital_write(5,0)
	p.digital_write(6,0)
	p.digital_write(7,0)
	

def position_lights(position):
#turn on the light indicating the currently selected position	
	if(position == 0):
		p.digital_write(0,1)
		p.digital_write(1,0)
		p.digital_write(2,0)
		p.digital_write(3,0)
		p.digital_write(4,0)
		p.digital_write(5,0)
		p.digital_write(6,0)
		p.digital_write(7,0)
	elif(position == 1):
		p.digital_write(0,0)
		p.digital_write(1,1)
		p.digital_write(2,0)
		p.digital_write(3,0)
		p.digital_write(4,0)
		p.digital_write(5,0)
		p.digital_write(6,0)
		p.digital_write(7,0)
	elif(position == 2):
		p.digital_write(0,0)
		p.digital_write(1,0)
		p.digital_write(2,1)
		p.digital_write(3,0)
		p.digital_write(4,0)
		p.digital_write(5,0)
		p.digital_write(6,0)
		p.digital_write(7,0)
	elif(position == 3):
		p.digital_write(0,0)
		p.digital_write(1,0)
		p.digital_write(2,0)
		p.digital_write(3,1)
		p.digital_write(4,0)
		p.digital_write(5,0)
		p.digital_write(6,0)
		p.digital_write(7,0)
	elif(position == 4):
		p.digital_write(0,0)
		p.digital_write(1,0)
		p.digital_write(2,0)
		p.digital_write(3,0)
		p.digital_write(4,1)
		p.digital_write(5,0)
		p.digital_write(6,0)
		p.digital_write(7,0)
	elif(position == 5):
		p.digital_write(0,0)
		p.digital_write(1,0)
		p.digital_write(2,0)
		p.digital_write(3,0)
		p.digital_write(4,0)
		p.digital_write(5,1)
		p.digital_write(6,0)
		p.digital_write(7,0)
	elif(position == 6):
		p.digital_write(0,0)
		p.digital_write(1,0)
		p.digital_write(2,0)
		p.digital_write(3,0)
		p.digital_write(4,0)
		p.digital_write(5,0)
		p.digital_write(6,1)
		p.digital_write(7,0)
	elif(position == 7):
		p.digital_write(0,0)
		p.digital_write(1,0)
		p.digital_write(2,0)
		p.digital_write(3,0)
		p.digital_write(4,0)
		p.digital_write(5,0)
		p.digital_write(6,0)
		p.digital_write(7,1)
	else:
		return 0
		

if __name__ == '__main__':
	while(1):
		button_monitor()
