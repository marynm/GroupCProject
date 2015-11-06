import RPi.GPIO as GPIO
import os
import time
import threading

class HardwareInterface(threading.Thread):
def_init_(self):
	super(HardwareInterface, self)._init_()
	self.setDaemon(True)
	self.Running = False
	self.Dispensing = False
	self.down_cb = []
	self.up_cb = []
	self.feed_cb = []
	
	
def Dispense(self):
	if not self.dispensing:
		self.step=0
		self.Dispensing = True
		for cb in self.feed_cb:
		cb()



