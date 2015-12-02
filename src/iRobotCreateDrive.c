//---------------------------------------------------------------------------
//	iRobot Create Movement code
//
// Problems and Solutions
// 1. Wait distance and delay are rerouting upon obstacles
// - Make a loop that continuously sends a very minimal distance (ie 1 mm)
// - Continous update of cur_x and cur_y
// - make a constant sensor check, stop loop if sensor hit
//
// 2. Bot only accepts ints, but computations take doubles
// - Keep a backup of doubles in code
// - change double to int for all bot communication
//
// 3. No seperate position tracker
// - Start out at 0.0
// - Have 360 degree, like a regular x/y graph
// - Start pointed out at 0 angle
//
//
//---------------------------------------------------------------------------
#include <termios.h>					// tcgetattr(),
#include <unistd.h>						// tcgetattr(),
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <sys/file.h>

#define PI 3.14159265

#define BAUDRATE B57600
#define SERPORT "/dev/ttyUSB0"
int fd, i, ch, pos, cur_x, cur_y, cur_Angle, dist_travel;
double cur_x_change, cur_y_change;
char ret;
char data[256];

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
    data[0] = 128;
    data[1] = 131;
    data[2] = 145;
    data[3] = 0;
    data[4] = 32;
    data[5] = 255;
    data[6] = 223;
    data[7] = 157;
    data[8] = 0;
    data[9] = deg;
    SendToCreate( fd, data, 10 );
}

void goRight(int deg)
{
    data[0] = 128;
    data[1] = 131;
    data[2] = 145;
    data[3] = 255;
    data[4] = 223;
    data[5] = 0;
    data[6] = 32;
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


main(int argc, char *argv[])
{
	struct 	termios tty;
	char    inputline[256];
	char	*result;
	int 	flags = fcntl(STDIN_FILENO, F_GETFL);
	int	pos1_x, pos1_y, pos2_x, pos2_y, pos3_x, pos3_y, pos4_x, pos4_y;

	printf( "\niRobot Create Control Application by Eric Gregori" );

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
	cur_x = 0;
	cur_y = 0;
	cur_Angle = 0;
	pos1_x = 120;
	pos1_y = 100;
	pos2_x = 200;
	pos2_y = 300;
	pos3_x = 500;
	pos3_y = 500;
	pos4_x = 100;
	pos4_y = 700;

	while( 1 )
	{
		usleep( 250000 );
		result = fgets( inputline, 256, stdin );
		if( result == 0 ) continue;
		printf( "\ninputline=%s", inputline );
		if( inputline[0] == '^' )
		{
			if( isdigit( inputline[1] ) && isdigit( inputline[2] ) && isdigit( inputline[3] ) )
			{
				pos = ((inputline[1]-0x30)*100) +
				      ((inputline[2]-0x30)*10) +
				      ((inputline[3]-0x30));
				if(pos == 500)
				{
				    goForward(300);
				    continue;
				}

				//Go to Position One
				if(pos == 001)
				{
					PositionMover(pos1_x, pos1_y);
					stop();
				}

				//Go to Position Two
				if(pos == 002)
				{
					PositionMover(pos2_x, pos2_y);
					stop();
				}
				
				//Go to Position Three
				if(pos == 003)
				{
					PositionMover(pos3_x, pos3_y);
					stop();
				}
				
				//Go to Position Four
				if(pos == 004)
				{
					PositionMover(pos4_x, pos4_y);
					stop();
				}

				if(pos == 777)
				{
					// send STOP
		            stop();
					continue;
				}

				if( pos == 420)
				{
					// turn left
					goLeft(90);
					printf( "\nleft" );
					continue;
				}

				if( pos == 666 )
				{
					// trun right
					goRight(90);
					printf( "\nright" );
					continue;
				}/*

				// stop
	                	data[0] = 128;
				data[1] = 131;
				data[2] = 145;
				data[3] = 0;
				data[4] = 0;
				data[5] = 0;
				data[6] = 0;
				SendToCreate( fd, data, 7 );
				printf( "\nstop" ); */
			}
		}
	}
	close( fd );
}
