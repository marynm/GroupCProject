#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

void takePicture();
int pic_num = 0;
int song_num = 0;
char out[BUFSIZ];

int main()
{
    takePicture();
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

char** init_music()
{
    DIR *dir;
    struct dirent *entry;

    if(!(dir = opendir("./Music/")))
	return -1;
    while(entry = readdir(dir) != NULL)
    {
	song_num++;
    }
    closedir(dir);
    
    char song_list[song_num][BUFSIZ];
    int cur_song = 0;

    if(!(dir = opendir("./Music/")))
	return -1;;
    while(entry = readdir(dir) != NULL)
    {
	strcpy(song_list[cur_song],entry->d_name);
	cur_song++;
    }
    return song_list;
}
