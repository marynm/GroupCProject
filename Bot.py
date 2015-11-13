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
	print("Taking picture")

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



def get_files_by_file_size(dirname, reverse=False):
    """ Return list of file paths in directory sorted by file size """

    # Get list of files
    filepaths = []
    for basename in os.listdir(dirname):
        filename = os.path.join(dirname, basename)
        if os.path.isfile(filename):
            filepaths.append(filename)

    # Re-populate list with filename, size tuples
    for i in xrange(len(filepaths)):
        filepaths[i] = (filepaths[i], os.path.getsize(filepaths[i]))

    # Sort list by file size
    # If reverse=True sort from largest to smallest
    # If reverse=False sort from smallest to largest
    filepaths.sort(key=lambda filename: filename[1], reverse=reverse)

    # Re-populate list with just filenames
    for i in xrange(len(filepaths)):
        filepaths[i] = filepaths[i][0]

    return filepaths






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




  



  
