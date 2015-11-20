#!/usr/bin/env python
import socket
import os
import sys
   

def receive_command():
#wait for commands from the controller and process them
	while(1):
		print("test1")
		data = s.recv(BUFFER_SIZE)
		print("test2")
		if(data):
			command = data.decode("utf-8")
			print("received data:", command)

			if (command == "camera"):
				take_picture()
	
			elif (command == "music"):
				next_song()
		
			else:
				move_bot(command)
		


def move_bot(position):
#move to location and tell controller when reached"""
	print("Moving bot to position" + position);
	
	#once at position send message to controller indicating it has arrived
	#MESSAGE = bytes("at location", "utf-8")
	#s.send(MESSAGE)


def take_picture():
#take pictures"""
#fswebcam image.jpg
	print("Taking picture")
	
	#code to take and save picture in a 'Pictures' directory
	

def play_song(song):
	
	if(song in directoryList):
		print('Playing: ' + str(song))
		os.system('mplayer ' + song)
		return 'Playing:' + str(song)
	else:
		print('Song "' + song + '" does not exist.')
		return 'Song "' + song + '" does not exist.'


def next_song():
	print("Playing next song")
	#move to next item in the directory list
	#song = directoryList[i] 	#?????
	#play_song(song)



def get_files(dirname):
    #Return list of file paths in directory

    # Get list of files
    filepaths = []
    for basename in os.listdir(dirname):
        filename = os.path.join(dirname, basename)
        if os.path.isfile(filename):
            filepaths.append(filename)

    return filepaths


if __name__ == '__main__':
	#setup TCP connection
	TCP_IP = '10.0.0.32'
	TCP_PORT = 5002
	BUFFER_SIZE = 1024

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((TCP_IP, TCP_PORT))
	
	#prepare the list of songs
	#directory = "./Music"		#????
	#directoryList = get_files(directory) #os.listdir(directory)
	
	while 1:
		receive_command()




  



  
