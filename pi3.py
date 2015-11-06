from time import sleep
import pifacedigitalio as p
import socket

#set up connection to server, with server's IP
TCP_IP = '10.0.0.21'
TCP_PORT = 5005
BUFFER_SIZE = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

#set up for lights/buttons
p.init()
x = 0	#indicates if a button has been pressed

#initialize with all lights on, indicating that the external IP is displayed
p.digital_write(0,1)
p.digital_write(1,1)
p.digital_write(2,1)


while(1):
	
	while(x!=1):
		if(p.digital_read(0)):
			x = 1
			sleep(1)
			p.digital_write(0,0)
			s.send(bytes("L", "utf-8"))	#send indication that 'Left' button was pressed
		elif(p.digital_read(1)):
			x = 1
			sleep(1)
			p.digital_write(1,0)
			s.send(bytes("R", "utf-8"))	#send indication that 'Right' button was pressed
	
	x = 0
	#receive and decode response from server
	data = s.recv(BUFFER_SIZE)
	data = data.decode("utf-8")

	if(data == "0"):		#External IP all three lights on
		p.digital_write(0,1)
		p.digital_write(1,1)
		p.digital_write(2,1)
	elif(data == "1"):		#Pi1 IP: 1st light on
		p.digital_write(0,1)
		p.digital_write(1,0)
		p.digital_write(2,0)
	elif(data == "2"):		#Pi2 IP: 2nd light on
		p.digital_write(0,0)
		p.digital_write(1,1)
		p.digital_write(2,0)
	elif(data == "3"):		#Pi3 IP: 3rd lights on
		p.digital_write(0,0)
		p.digital_write(1,0)
		p.digital_write(2,1)