//---------------------------------------------------------------------------
//	iRobot Create Control Application.
// 	-Used to demonstrate and show the iRobot's functionality
//	-The code used will be vital to final Bot code
//
//---------------------------------------------------------------------------
#include <termios.h>					// tcgetattr(),
#include <unistd.h>						// tcgetattr(),
#include <stdio.h>
#include <errno.h>
#include <sys/file.h>

//Setting up the iRobot serial communication
#define BAUDRATE B57600
#define SERPORT "/dev/ttyUSB0"


//A function to physically send the data to the Bot
void SendToCreate( int fd, char *data, int length )
{
	int i;

	for( i=0; i<length; i++ )
	{
		if( read(fd, &data[i], 1) == -1 )
		{
  			printf( "\nUnable to write %s", SERPORT );
			printf( "\nerrno = %d", errno );
		}
		usleep( 5000 );
	}
}


void ReadFromCreate( int fd)
{
	int i;
	char data1 [500];

	for( i=0; i<500; i++ )
	{
		if( read(fd, &data1[i], 1) == -1 )
		{
  			printf( "\nUnable to read %s", SERPORT );
			printf( "\nerrno = %d", errno );
		}
		printf("\n%d", data1[i]);
		usleep( 5000 );
	}
}


main(int argc, char *argv[])
{
	int	fd, i, x, ch, stoptimer, pos;
	struct 	termios tty;
	char    inputline[256];
	char	data[256];
	char	data1[500];
	char	*result;
	int 	flags = fcntl(STDIN_FILENO, F_GETFL);


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
	while( 1 )
	{
		usleep( 250000 );
		//printf( "\nWaiting" );
		result = fgets( inputline, 256, stdin );
		if( result == 0 ) continue;
		printf( "\ninputline=%s", inputline );
		if( inputline[0] == '^' )
		{
			stoptimer = 0;
			if( isdigit( inputline[1] ) && isdigit( inputline[2] ) && isdigit( inputline[3] ) )
			{
				pos = ((inputline[1]-0x30)*100) +
				      ((inputline[2]-0x30)*10) +
				      ((inputline[3]-0x30));

				//Go Forward
				printf( "\ntest1" );
				if(pos == 001){
					data[0] = 128;
					data[1] = 131;
					data[2] = 137;
					data[3] = 0;
					data[4] = 100;
					data[5] = 128;
					data[6] = 0;
					SendToCreate( fd, data, 7 );
				}

				if(pos == 002)
				{
					// send STOP
		      data[0] = 128;
					data[1] = 131;
					data[2] = 145;
					data[3] = 0;
					data[4] = 0;
					data[5] = 0;
					data[6] = 0;
					SendToCreate( fd, data, 7 );
					continue;
				}

				if( pos == 003)
				{
					// turn left
          data[0] = 128;
					data[1] = 131;
					data[2] = 145;
					data[3] = 0;
					data[4] = 32;
					data[5] = 255;
					data[6] = 223;
					SendToCreate( fd, data, 7 );
					printf( "\nleft" );
					continue;
				}

				if( pos == 004)
				{
					// turn right
          data[0] = 128;
					data[1] = 131;
					data[2] = 145;
					data[3] = 255;
					data[4] = 223;
					data[5] = 0;
					data[6] = 32;
					SendToCreate( fd, data, 7 );
					printf( "\nright" );
					continue;
				}
				for( x=0; x<30; x++ )
				{
					ReadFromCreate(fd);
					sleep(1);
				}
			}
		}

	}

	close( fd );
}
