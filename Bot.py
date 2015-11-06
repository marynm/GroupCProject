#!/usr/bin/env python
import socket
import os
import sys
   

def receive_command():
	while(1):
		#data = s.recv(BUFFER_SIZE)
		#command = data.decode("utf-8")
		command = "3"
		print("received data:", command)

		if (command == "camera"):
			take_picture()
	
		elif (command == "music"):
			play_next_song()
		
		else:
			move_bot(command)
		


def move_bot(position):
#move to location and tell controller when reached
	print("Moving bot to position" + position);


def take_picture():
#take pictures
#fswebcam image.jpg
	print("Take picture")

def setup_music():
#change songsprint ("received data:", data.decode("utf-8"))

	directory = "." #sys.argv[1]
	directoryList = os.listdir(directory)

def play_song(song):
	
	if(song in directoryList):
		print('Playing: ' + str(song))
		os.system('mplayer ' + song)
		return 'Playing:' + str(song)
	else:
		print('Song "' + song + '" does not exist.')
		return 'Song "' + song + '" does not exist.'

def play_next_song():
	print("Playing next song")

if __name__ == '__main__':
	#setup TCP connection
	TCP_IP = '10.0.0.32'
	TCP_PORT = 5005
	BUFFER_SIZE = 1024

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	#s.connect((TCP_IP, TCP_PORT))
	
	while 1:
		receive_command()
		#play_song(input('song pls'))




  



  