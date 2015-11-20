import controller
import unittest
from time import sleep
import pifacedigitalio as p

class TestSequenceFunctions(unittest.TestCase):

	def test_position_lights(self):
		#cycle through all the position lights, moving up, roll-over, adn back down
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
		

	def test_flash_lights(self):
		#test the function that flashes the lights on and off, and the function that reestablishes the position light
		self.assertEqual(controller.position_lights(3), 3)
		controller.flash_lights()
		self.assertEqual(controller.position_lights(3), 3)

	
	def test_move_bot_function(self):
		#send the position to the bot and return the position
		self.assertEqual(controller.move_bot(5), 5)
		self.assertEqual(controller.move_bot(0), 0)
		self.assertEqual(controller.move_bot(7), 7)

	def test_camera_and_songs(self):
		#test the song and camera functions (Note: without acually sending message this is pretty much an empty test..)
		print("Testing the camer and change song functions: ")
		controller.take_picture()
		controller.change_song()
		
	

if __name__ == '__main__':
    unittest.main()
