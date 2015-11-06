import os
import sys

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

if __name__ == '__main__':
	while 1:
		play_song(input('song pls'))
