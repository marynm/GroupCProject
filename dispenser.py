import socket
import os

UDP_IP = "10.0.0.20"
UDP_PORT = 5004
sock = socket.socket(socket.AF_INET, # Internet
    socket.SOCK_DGRAM) # UDP
sock.bind((UDP_IP, UDP_PORT))

	while True:
      data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
	  data = data.decode("utf-8")
	  if (data == "bot")
			activate_dispensor()
			
			
			
			
			
def activate_dispensor():
#activates the sensor to wait for bot to arrive
	print("Activating dispensor...waiting for bot")
	
	#turn on sensor
	
	while(1)
		#monitor sensor, waiting for bot to arrive
		
		#when sensor activated, turn motor to open dispenser
		
		#turn motore to close dispenser after x seconds
	

