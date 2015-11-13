import socket
import os

			
			
			
			
			
def activate_dispensor():
#activates the sensor to wait for bot to arrive
	print("Activating dispensor...waiting for bot")
	
	#turn on sensor
	
	
	#monitor sensor, waiting for bot to arrive
		
	#when sensor activated, turn motor to open dispenser
		
	#turn motor to close dispenser after x seconds
		
	#disactivate sensor
		
		
		
if __name__ == '__main__':
	#prepare to receive UPD messages from controller
	UDP_IP = "10.0.0.20"
	UDP_PORT = 5004
	sock = socket.socket(socket.AF_INET, # Internet
    socket.SOCK_DGRAM) # UDP
	sock.bind((UDP_IP, UDP_PORT))

	#wait for messages from cotroller tell the dispensor that the bot is coming#FFFFFF
	while(1):
      data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
	  data = data.decode("utf-8")
	  if (data == "dispenser"):
			activate_dispensor()		
	

