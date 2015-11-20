import socket
import RPi.GPIO as GPIO
import time



def dispense():
#open the dispenser door for 3 seconds, then reclose it
		p.ChangeDutyCycle(5)
		time.sleep(3)
		p.ChangeDutyCycle(12.5)

if __name__ == '__main__':

	UDP_IP = "10.0.0.21"
	UDP_PORT = 5005

	
	GPIO.setmode(GPIO.BOARD)

	GPIO.setup(7, GPIO.OUT)

	p = GPIO.PWM(7,50)
	p.start(12.5)

	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
	sock.bind((UDP_IP, UDP_PORT))

	try:

		while True:
			data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
			print( "received message:" + data.decode("utf-8"))
			dispense()

	except KeyboardInterrupt:
		p.stop()
	
		GPIO.cleanup()
