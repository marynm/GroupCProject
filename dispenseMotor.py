
import RPi.GPIO as GPIO
import time

def dispense():
#open the dispenser door for 3 seconds, then reclose it
		p.ChangeDutyCycle(5)
		time.sleep(3)
		p.ChangeDutyCycle(12.5)



if __name__ == '__main__':

	GPIO.setmode(GPIO.BOARD)

	GPIO.setup(7, GPIO.OUT)

	p = GPIO.PWM(7,50)
	p.start(7.5)
	try:
		while(1):
			dispense()
			time.sleep(5)

	except KeyboardInterrupt:
		p.stop()
	
		GPIO.cleanup()