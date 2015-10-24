import controller
import unittest
from time import sleep

class TestSequenceFunctions(unittest.TestCase):

	def test_position_lights(self):
		#cycle through all the position lights, moving up, roll-over, and back down
		i = 0
		while(i!=8):
			controller.position_lights(i)
			i = i + 1
			sleep(0.2)
		controller.position_lights(0)
		sleep(0.2)
		i = 7
		while(i!=-1):
			controller.position_lights(i)
			i = i - 1
			sleep(0.2)
	def test_position_lights_positionDNE(self):
		#test the light control function with unexpected input value
		self.assertEqual(controller.position_lights(8), 0)
	
	def test_move_bot_function(self):
		#this function is currently not written, but it should send the position to the bot
		self.assertEqual(controller.move_bot(5), 5)
		self.assertEqual(controller.move_bot(0), 0)
		self.assertEqual(controller.move_bot(7), 7)

if __name__ == '__main__':
    unittest.main()