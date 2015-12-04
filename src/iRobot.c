#include <sys/types.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <termios.h>					// tcgetattr(),
#include <unistd.h>						// tcgetattr(),
#include <math.h>

#define FALSE 0;
#define TRUE 1;
#define PI 3.14159265
#define BAUDRATE B57600
#define SERPORT "/dev/ttyUSB0"

int fd, i, ch, pos, cur_x, cur_y, cur_Angle, dist_travel, location;
int msg_size = 20;
int pic_num = 0;
int song_num = 0;
int mystop = 0;
int list_size = -2;//starts at -2 to account for . and ..
double cur_x_change, cur_y_change;
char ret;
char data[256];//this is the data to pass to the robot
char running = FALSE;
char out[BUFSIZ];//this is a general string used for executing system commands
char playing_song[BUFSIZ];//this is to pass the current song to the music player thread
char control_line[256];//this is to pass the iRobot's movment commands to the thread

void SendToCreate( int fd, char *data, int length )
{
	int i;

	for( i=0; i<length; i++ )
	{
		if( write(fd, &data[i], 1) == -1 )
		{
  			printf( "\nUnable to write %s", SERPORT );
			printf( "\nerrno = %d", errno );
		}
		usleep( 5000 );
	}
}

char ReceiveFromCreate(int fd)
{
	if(read(fd, &ret, 1) == -1)
	{
		printf("unable to read\n");
	}
	ret &= 0x03;
	return ret;
}

void ChangedPos(int Howfar)
{
	if (cur_Angle <= 90)
	{
		cur_x_change = (double) cur_x + (cos(cur_Angle * PI / 180) * (double) Howfar);
		cur_y_change = (double) cur_y + (sin(cur_Angle * PI / 180) * (double) Howfar);
	}
	else if(cur_Angle <= 180)
	{
		cur_x_change = (double) cur_x - (cos((180 - cur_Angle) * PI / 180) * (double) Howfar);
		cur_y_change = (double) cur_y + (sin((180 - cur_Angle) * PI / 180) * (double) Howfar);
	}
	else if(cur_Angle <= 270)
	{
		cur_x_change = (double) cur_x - (cos((cur_Angle - 180) * PI / 180) * (double) Howfar);
		cur_y_change = (double) cur_y - (sin((cur_Angle - 180) * PI / 180) * (double) Howfar);
	}
	else
	{
		cur_x_change = (double) cur_x + (cos((360 - cur_Angle) * PI / 180) * (double) Howfar);
		cur_y_change = (double) cur_y - (sin((360 - cur_Angle) * PI / 180) * (double) Howfar);
	}
}

void AdjustPosition()
{
	ChangedPos(dist_travel);
	printf("The current X position is %d\n", cur_x);
	printf("The current Y position is now %d\n", cur_y);
	printf("The x Position is changing by %f\n", cur_x_change);
	printf("The y Position is changing by %f\n", cur_y_change);
	cur_x = (int)floor(cur_x_change);
	cur_y = (int)floor(cur_y_change);
	printf("The changed X position is now %d\n", cur_x);
	printf("The changed Y position is now %d\n", cur_y);
}

void ObstacleHandler()
{
	int ObstacleDist = 300;
	printf("Obs(1) angle is %d\n", cur_Angle);
	if(ret == 2)
	{
		goRight(90);
		if(cur_Angle > 90)
		{
			cur_Angle = cur_Angle - 90;
		}
		else
		{
			cur_Angle = 360 + cur_Angle - 90;
		}
	}
	else
	{
		goLeft(90);
		if(cur_Angle >= 270)
		{
			cur_Angle = cur_Angle + 90 - 360;
		}
		else
		{
			cur_Angle = cur_Angle + 90;
		}
	}

	//Error Checking The goForward
	ChangedPos(ObstacleDist);
	while(cur_x_change < 0 || cur_y_change < 0)
	{
		ObstacleDist = ObstacleDist - 5;
		ChangedPos(ObstacleDist);
		if(ObstacleDist <= 0)
		{
			break;
		}
	}

	printf("Obs(2) angle is %d\n", cur_Angle);

	if(ObstacleDist <= 0)
	{
		printf("Error, cannot move around obstacle");
	}
	else
	{
		goForward(ObstacleDist);
		AdjustPosition();
		if(dist_travel < ObstacleDist)
		{
			ObstacleHandler();
		}
	}
}

void PositionMover(int pos_x, int pos_y)
{
	//Get the Hypotenuse and Angle
	double dist;
	double angel;
	int difx;
	int dify;
	double difx2;
	double dify2;
	double toDeg = 180 / PI;
	while(((pos_x + 3) < cur_x || (pos_x - 3) > cur_x) && (cur_y < (pos_y - 3) || cur_y > (pos_y + 3)))
	{

		dify = abs(cur_y-pos_y);
		difx = abs(cur_x-pos_x);
		difx2 = (double)(difx);
		dify2 = (double)(dify);
		dist = hypot( abs(cur_x-pos_x),  abs(cur_y-pos_y));
  		angel =  atan(dify2 / difx2) * toDeg;

		int dist_use = (int)floor(dist);

		int angle_use = (int)floor(angel);
		printf("Current Angle is %d\n", cur_Angle);
		printf("Angle Using is %d\n", angle_use);

		if(cur_x == pos_y && cur_y == pos_y)
		{
			//Do nothing
		}
		else if(cur_Angle == angle_use)
		{
			goForward(dist_use);
		}

		else if (cur_x < pos_x)
		{
			if(cur_y < pos_y)
			{
				//Upper Right
				//Find Angle
				if(cur_Angle < angle_use)
				{
					goLeft(angle_use - cur_Angle);
				}
				else if(cur_Angle < 180)
				{
					goRight(cur_Angle - angle_use);
				}
				else
				{
					goLeft(360 - cur_Angle + angle_use);
				}
				cur_Angle = angle_use;
				//Give hypotenuse
				goForward(dist_use);
				AdjustPosition();
				if(dist_travel < dist_use)
				{
					ObstacleHandler();
				}
			}
			else
			{
				//Lower Right
				//Find Angle
				if(cur_Angle > (360 - angle_use))
				{
					goRight(cur_Angle - (360 - angle_use));
				}
				else if(cur_Angle > 90)
				{
					goLeft(360 - angle_use - cur_Angle);
				}
				else
				{
					goRight(cur_Angle + angle_use);
				}
				cur_Angle = 360 - angle_use;
				//Give hypotenuse
				goForward(dist_use);
				AdjustPosition();
				if(dist_travel < dist_use)
				{
					ObstacleHandler();
				}
			}
		}

		else
		{
			if(cur_y < pos_y)
			{
				//Upper Left
				//Find Angle
				if(cur_Angle < ((90 - angle_use) + 90))
				{
					goLeft(180 - angle_use - cur_Angle);
				}
				else
				{
					goRight(cur_Angle - 180 + angle_use);
				}
				cur_Angle = 180 - angle_use;
				//Give hypotenuse
				goForward(dist_use);
				AdjustPosition();
				if(dist_travel < dist_use)
				{
					ObstacleHandler();
				}
			}
			else
			{
				//Lower Left
				//Find Angle

				if(cur_Angle < (angle_use + 180))
				{
					goLeft((angle_use + 180) - cur_Angle);
				}
				else
				{
					goRight(cur_Angle - (angle_use + 180));
				}
				cur_Angle = angle_use + 180;
				//Give hypotenuse
				goForward(dist_use);
				AdjustPosition();
				if(dist_travel < dist_use)
				{
					ObstacleHandler();
				}
			}
		}
	}
}

char polldata()
{
	//this will request sensor data from the bot
	data[0] = 128;
	data[1] = 131;
	data[2] = 142;
	data[3] = 7;
	SendToCreate(fd, data, 4);
	return ReceiveFromCreate(fd);
}

void goLeft(int deg)
{
	if (deg > 4)
	{
		deg = deg - 4;
	}
    data[0] = 128;
    data[1] = 131;
    data[2] = 137;
    data[3] = 0;
    data[4] = 32;
    data[5] = 0;
    data[6] = 1;
    data[7] = 157;
    data[8] = 0;
    data[9] = deg;
    SendToCreate( fd, data, 10 );
}

void goRight(int deg)
{
	if (deg > 4)
	{
		deg = deg - 4;
	}
    data[0] = 128;
    data[1] = 131;
    data[2] = 137;
    data[3] = 255;
    data[4] = 223;
    data[5] = 0xFF;
    data[6] = 0xFF;
    data[7] = 157;
    data[8] = 0xFF;
    data[9] = (deg ^ 0xFF) + 1;
    SendToCreate( fd, data, 10 );
}

void goForward(int dist)
{
	for(dist_travel = 0;dist_travel < dist;dist_travel += 5)
	{
    	data[0] = 128;
    	data[1] = 131;
    	data[2] = 137;
    	data[3] = 0;
    	data[4] = 100;
    	data[5] = 128;
    	data[6] = 0;
    	data[7] = 156;
    	data[8] = 0;
    	data[9] = 5;
    	SendToCreate( fd, data, 10 );
		if(polldata())
			break;
	}
	stop();
}

void stop()
{
	data[0] = 128;
	data[1] = 131;
	data[2] = 145;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;
	SendToCreate( fd, data, 7 );
}

void superstop()
{
	data[0] = 128;
	data[1] = 132;
	data[2] = 145;
	data[3] = 0;
	data[4] = 0;
	data[5] = 0;
	data[6] = 0;
	SendToCreate( fd, data, 7 );
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
	strcpy(playing_song, song_list[song_num]);
	return 0;
}

void play_song()
{
	printf("in play_song\n");
	sprintf(out,"mplayer ./music/%s",playing_song);
	system(out);
	song_num++;
}

void next_song(char song_list[list_size][BUFSIZ])
{
	if(song_num < list_size)
	{
		song_num++;
		strcpy(playing_song, song_list[song_num]);
	}
}

void *automated_pictures(void *arg)
{
	while(running)
	{
		printf("took picture\n");
		sleep(30);
		takePicture();
	}
	pthread_exit("Automated Pictures Exit");
}

void *music_player(void *arg)
{
	printf("in music thread\n");
	play_song();
	pthread_exit("Music Player Exit");
}

void *robot_control(void *arg)
{
	struct 	termios tty;
	char	*result;
	int 	flags = fcntl(STDIN_FILENO, F_GETFL);
	int	pos1_x, pos1_y, pos2_x, pos2_y, pos3_x, pos3_y, pos4_x, pos4_y, pos5_x, pos5_y, pos6_x, pos6_y, pos7_x, pos7_y, pos8_x, pos8_y;

	printf( "\nOpeneing serial port: %s", SERPORT );
	fd = open(SERPORT, O_RDWR);      			//open the serial port

   	if ( fd < 0 )
   	{
    printf( "\nUnable to open %s", SERPORT );
		printf( "\nerrno = %d", errno );
		exit(0);
   	}
	else
		printf( "\nSuccessfully opened the serial port" );

	tcgetattr(fd, &tty);						// Read port parameters into termios structure

	// cc_t     c_cc[NCCS];   /* control chars */

	tty.c_cflag = CLOCAL | CREAD | CS8;			// CLOCAL 	= ignore modem control lines
												// CREAD  	= enable receiver
												// CS8    	= 8 bit data
												// ~CRTSCTS	= no hardware flow control

	tty.c_iflag = IGNBRK | IGNPAR;				// IGNBRK	= ignore BREAK condition
												// IGNPAR	= ignore framing anf parity errors


	tty.c_oflag = 0;							//
	tty.c_lflag = 0;							//

	cfsetospeed(&tty, BAUDRATE );				// Set output baudrate
	cfsetispeed(&tty, BAUDRATE );				// set input baudrate

	if( tcsetattr(fd, TCSANOW, &tty) < 0 )		// Set serial port - TCSANOW = Apply changes immediately
	{
      	printf( "\nUnable to tcsetattr %s", SERPORT );
		printf( "\nerrno = %d", errno );
	}
	else
		printf( "\ntcsetattr successfull" );

	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);


	//Define coordinates
	cur_x = 50;
	cur_y = 50;
	cur_Angle = 0;
	pos1_x = 50;
	pos1_y = 50;
	pos2_x = 600;
	pos2_y = 600;
	pos3_x = 300;
	pos3_y = 300;
	pos4_x = 900;
	pos4_y = 300;
	pos5_x = 666;
	pos5_y = 666;
	pos6_x = 198;
	pos6_y = 234;
	pos7_x = 345;
	pos7_y = 645;

	while(running)
	{
		usleep( 250000 );
		if(location)
		{
			if(!mystop)
			{
				//Go to Dispenser
				if(location == 8)
				{
					mystop = 1;
					PositionMover(pos1_x, pos1_y);
					stop();
					mystop = 0;
				}
				
				if(location == 7)
				{
					mystop = 1;
					PositionMover(pos1_x, pos1_y);
					stop();
					mystop = 0;
				}

				//Go to Position Two
				if(location == 2)
				{
					mystop = 1;
					PositionMover(pos2_x, pos2_y);
					stop();
					mystop = 0;
				}

				//Go to Position Three
				if(location == 3)
				{
					mystop = 1;
					PositionMover(pos3_x, pos3_y);
					stop();
					mystop = 0;
				}

				//Go to Position Four
				if(location == 4)
				{
					mystop = 1;
					PositionMover(pos4_x, pos4_y);
					stop();
					mystop = 0;
				}
					
				//Go to Position Five
				if(location == 5)
				{
					mystop = 1;
					PositionMover(pos5_x, pos5_y);
					stop();
					mystop = 0;
				}
					
				//Go to Position Six
				if(location == 6)
				{
					mystop = 1;
					superstop();
					mystop = 0;
				}
					
				//Go to Fuck-It
				if(location == 1)
				{
					pos8_x = rand()%1000 + 1;
					pos8_y = rand()%1000 + 1;
					PositionMover(pos4_x, pos4_y);
					stop();
					mystop = 0;
				}
			}
			else
			{
				sleep(1);
			}
		}
	}
	pthread_exit("Robot Control Exit");
}

int main()
{
	running = TRUE;
	pthread_t aP, m, rC;
	void *res;
	int s;
	DIR *dir;
	struct dirent *entry;
	
	int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    char command[msg_size];

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

	if(!(dir = opendir("./music/")))
	{
		return -1;
	}
	while((entry = readdir(dir)) != NULL)
	{
		list_size++;
	}
	closedir(dir);
	printf("size is: %i\n",list_size);
	char song_list[list_size][BUFSIZ];
	init_music(song_list);
	s = pthread_create(&aP, NULL, automated_pictures, (void *) 0);
	if(s != 0)
	{
		printf("Error pthread_create aP returned: %i\n", s);
	}
	s = pthread_create(&m, NULL, music_player, (void *) 0);
	if(s != 0)
	{
		printf("Error pthread_create m returned: %i\n", s);
	}
	s = pthread_create(&rC, NULL, robot_control, (void *) 0);
	if(s != 0)
	{
		printf("Error pthread_create rC returned: %i\n", s);
	}
	while(running)
	{
		//constantly monitor for messages
		read(sockfd, &command, 1024);
		int option = 20;
		//get option from network
		if(strncmp(command, "done", 4) == 0)
		{
				if(!mystop)
				{
					strcpy(command, "yes");
					printf("Bot done. Sending msg %s\n", command);
					write(sockfd, &command, 1024);
				}
				else
				{
					strcpy(command, "no");
					printf("sending message: %s\n");
					write(sockfd, &command, 1024);
				}
		}
		else if(strncmp(command, "camera", 6) == 0)//manually take a picture
		{
			takePicture();
		}
		else if(strncmp(command, "music", 5) == 0)//choose next song
		{
			next_song(song_list);
			s = pthread_cancel(m);
			if(s != 0)
			{
				printf("Error pthread_cancel m returned: %i\n", s);
			}
			s = pthread_join(m, &res);//this pthread_join must be in loop for music player to work
			if(s != 0)
			{
				printf("Error pthread_join m returned: %i\n", s);
			}
			s = pthread_create(&m, NULL, music_player, (void *) 0);
			if(s != 0)
			{
				printf("Error pthread_create m returned: %i\n", s);
			}
		}
		else //command the bot
		{
			mystop = 0;
			location = NULL;
			location = atoi(command) + 1;
		}
	}
	s = pthread_join(aP, &res);
	if(s != 0)
	{
		printf("Error pthread_join aP returned: %i\n", s);
	}
	s = pthread_join(rC, &res);
	if(s != 0)
	{
		printf("Error pthread_join rC returned: %i\n", s);
	}
	return 0;
}
