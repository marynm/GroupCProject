import os
import sys

directory = "." #sys.argv[1]
directoryList = os.listdir(path=directory)

def play_song(song):
	
	if(song in directoryList):
		return 'Playing:' + str(song)
		#os.system('mplayer ' + song)
	else:
		return 'Song "' + song + '" does not exist.'
#while 1:
	#play_song(input('song pls'))
