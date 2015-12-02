#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int pic_num = 0;
int song_num = 0;
int list_size = -2;//starts at -2 to account for . and ..
char out[BUFSIZ];

int main()
{
	DIR *dir;
	struct dirent *entry;

	if(!(dir = opendir("./music/")))
	{
		return -1;
	}
	while(entry = readdir(dir) != NULL)
	{
		list_size++;
	}
	closedir(dir);
	printf("size is: %i\n",list_size);
	char song_list[list_size][BUFSIZ];
	init_music(song_list);
	//takePicture();
	return 0;
}

void takePicture()
{
	//create a linux command string to take a picture using a webcam and properly date them
	sprintf(out,"fswebcam -r 1280x720 --no-banner ./pictures/image%i.jpg",pic_num);
	//system will execute linux command created above
	system(out);
	pic_num++;
}

int init_music(char song_list[list_size][BUFSIZ])
{
	DIR *dir;
	struct dirent *entry;
	int cur_song = 0;
	int ignore_dir = 2;
	if(!(dir = opendir("./music/")))
	{
		return -1;
	}
	while((entry = readdir(dir)) != NULL)
	{
		if(ignore_dir)//to ignore . and ..
		{
			ignore_dir--;
		}
		else
		{
			strcpy(song_list[cur_song],entry->d_name);
			cur_song++;
		}
	}
	closedir(dir);
	return 0;
}

void play_song(char song_list[list_size][BUFSIZ])
{
	sprintf(out,"mplayer ./music/%s",song_list[song_num]);
	system(out);
	song_num++;
}

void next_song(char song_list[list_size][BUFSIZ])
{
	if(song_num < list_size)
	{
		song_num++;
		play_song(song_list);
	}
}
