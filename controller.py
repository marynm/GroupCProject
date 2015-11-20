#The controller pi, used as the 'remote' to direct the bot actions (moving, food fill up, music control)

import pifacedigitalio as p
from time import sleep
import socket

#initial basic functionality:
	#When the button to scroll is pressed the lights move to indicate which of the 8 possible pre-programmed loactions the bot will move to (one of which is the dispensor)
	#When the select camera, or song button is pressed, the apllicable message is sent to the bot adn all of the lights flash once



def button_monitor():
	position = 0	#keeps track of the current position of the bot
	x = 0
	msg_received = 0

	while(1):
		if(p.digital_read(0)):			#button 0 pressed: move to next postion 'up'
			sleep(0.5)
			position = position + 1
			if(position == 8):		#check for roll-over
				position = 0
			position_lights(position)	#update the lights

		elif(p.digital_read(1)):		#button 1 pressed: confirm current selection
			sleep(0.5)
			move_bot(position)		#tell bot to move to the selected location
			if(check_bot_done()):			#while the bot is still carrying out the previous move command it can't take the new position request
				flash_lights()			#flash lights to indicate a button press
				position_lights(position)	#then return to indicating the position

			if(position == 7):		#if the bot is going to the dispenser, alert the despiensor to expect the bot
				alert_dispenser()	
		
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




def move_bot(position):
#send message to bot telling it which position to go to
	
	send_message_to_bot(str(position))
	sleep(3)
	if(position != 7):
		print("Moving bot to location " + str(position))
	else:
		print("Moving bot to dispenser");

	return position



def take_picture():
#send message to bot telling it to take a picture
	print("Taking picture");
	send_message_to_bot("camera")


def change_song():
#send message to bot telling it to change the song
	print("Changing song");
	send_message_to_bot("music")

def alert_dispenser():
#Alert the deispenser that the bot is coming to that it is prepaered to dispense
	print("Alerting dispenser")
	MESSAGE = bytes("dispenser", "utf-8")
	sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))


def send_message_to_bot(msg):
#convert the passed message string to bytes and send the message to the bot
	print("Sending message '" + msg + "' to bot.")
	MESSAGE = bytes(msg, "utf-8")
	conn.send(MESSAGE)		#-> comment out for testing functions independently (without message connections set up)


def check_bot_done():
	#check if bot is still carrying out the last command
	data = conn.recv(BUFFER_SIZE)
	msg = data.decode("utf-8")
	if(msg == "done"):
		return 1
	else:
		return 0



def flash_lights():
#flash all of the lights on and off once
	p.init()
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
	p.init()
	if(position == 0):
		p.digital_write(0,1)
		p.digital_write(1,0)
		p.digital_write(2,0)
		p.digital_write(3,0)
		p.digital_write(4,0)
		p.digital_write(5,0)
		p.digital_write(6,0)
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
	
	return position


if __name__ == '__main__':
	p.init()

	#set up connection to clients (bot and dispenser), with controller's IP
	#TCP connection with bot
	TCP_IP = '10.0.0.32'
	TCP_PORT = 5002
	BUFFER_SIZE = 1024
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((TCP_IP, TCP_PORT))
	s.listen(1)
	conn, addr = s.accept()

	print("TCP connection set up")
	
	#UDP is used for communication with dispenser
	#setup_UDP()
	UDP_IP = "10.0.0.21"
	UDP_PORT = 5005
	sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
	
	#continually chack for and handle button presses
	try:	
		while(1):
			button_monitor()

	except KeyboardInterrupt:
		conn.close()


