#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

void takePicture();
int pic_num = 0;
int song_num = 0;
int list_size = 0;
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
	printf("3rd item in song list: %s\n",song_list[2]);
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

void init_music(char song_list[list_size][BUFSIZ])
{
    DIR *dir;
    struct dirent *entry;
    int cur_song = 0;
    if(!(dir = opendir("./music/")))
    {
		return -1;
	}
    while((entry = readdir(dir)) != NULL)
    {
		strcpy(song_list[cur_song],entry->d_name);
		cur_song++;
    }
    closedir(dir);
}
