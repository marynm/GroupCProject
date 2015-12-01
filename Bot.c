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

void takePicture();
void camera_thread_function();

int pic_num = 0;
int song_num = 0;
int list_size = -2; //starts at -2 to account for . and ..
char out[BUFSIZ];


int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char ch = 'A';

/*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = 9734;
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client2");
        exit(1);
    }

/*  We can now read/write via sockfd.  */

    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(sockfd);
    exit(0);

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

	//printf("size is: %i\n",list_size);
	char song_list[list_size][BUFSIZ];
	init_music(song_list);

    return 0;

    int res;
    pthread_t camera_thread;
    pthread_t communication_thread;
    void *thread_result;
 
   res = pthread_create(camera_thread, NULL, camera_thread_function, (void *)1);
	
	if (res != 0) {
	//	perror(“Thread creation failed”);
		exit(EXIT_FAILURE);
	}
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

void stop_song()
{
    //TODO
}

void next_song(char song_list[list_size][BUFSIZ])
{
    if(song_num < list_size)
    {
	stop_song();
	song_num++;
	play_song(song_list);
    }
}

void previous_song(char song_list[list_size][BUFSIZ])
{
    if(song_num >= 0)
    {
	stop_song();
	song_num--;
	play_song(song_list);
    }
}
void camera_thread_function()
{
	while(1){
		sleep(30);
		takePicture();
	}
}

