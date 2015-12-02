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
void music_thread_function();

int pic_num = 0;
int song_num = 0;
int list_size = -2; //starts at -2 to account for . and ..
char out[BUFSIZ];
int msg_size = 20;


int main()
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char command[msg_size];
    int location = 0;

/*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("10.0.0.32");
    address.sin_port = htons(5002);
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client");
        exit(1);
    }

/*  We can now read/write via sockfd.  */

	DIR *dir;
    struct dirent *entry;

    if(!(dir = opendir("./music/")))
    {
	printf("opendir mistake\n");
		return -1;
	}
    while(entry = readdir(dir) != NULL)
    {
		list_size++;
    }
    closedir(dir);

	char song_list[list_size][BUFSIZ];
	printf("init music");
	init_music(song_list);


    int res;
    pthread_t camera_thread;
    pthread_t communication_thread;
    void *thread_result;
    int moving = 0;
 
   /*res = pthread_create(camera_thread, NULL, camera_thread_function, (void *)1);

	if (res != 0) {
	//	perror(“Thread creation failed”);
		exit(EXIT_FAILURE);
	}
	*/

	//communication:
	while(1)
	{
		
		//constantly monitor for messages
		read(sockfd, &command, 1024);
		printf("Received msg: %s\n", command);
		//if received 'done' prompt, respond with yes or no
		if(strncmp(command, "done", 4) == 0)
		{
			//if not currently carrying out a command
			{
				strcpy(command, "yes");
				printf("Bot done. Sending msg %s\n", command);
				write(sockfd, &command, 1024);
			}
			//else
			{
				//strcpy(command, "no");
				//write(sockfd, &command, 1024);
			}
			
		}
		else if(strncmp(command, "camera", 6) == 0)
			takePicture();
		else if(strncmp(command, "music", 5) == 0)
			printf("changing song\n");//change song
		else //if received a message containing a number, move to the position corresponding to that number
		{
			location = atoi(command);
			printf("moving bot to location %d\n", location);
			//move bot to the location sent
		}	
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


