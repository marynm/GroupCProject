//---------------------------------------------------------------------------
//	iRobot Create Control Application
//	Written by Eric Gregori ( www.EMGRobotics.com )
//
//    Copyright (C) 2009  Eric Gregori
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// This is a Linux application for controlling the iRobot Create from a command line, or 
// linux STDIO pipe.  To use, simply pipe the output or your application into this application.
// sudo ./glview | iRobotCreateDrive
//
//
//---------------------------------------------------------------------------
#include <termios.h>					// tcgetattr(), 
#include <unistd.h>						// tcgetattr(), 
#include <stdio.h>
#include <errno.h>
#include <sys/file.h>

#define BAUDRATE B57600
#define SERPORT "/dev/ttyUSB0"


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



main(int argc, char *argv[])
{
	int	fd, i, ch, stoptimer, pos;
	struct 	termios tty;
	char    inputline[256];
	char	data[256];
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
	stoptimer = 0;
	while( 1 )
	{
		stoptimer++;
		/*if( stoptimer >= 2 )
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
		}*/
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
				if(pos == 500){
					data[0] = 128;
					data[1] = 131;
					data[2] = 137;
					data[3] = 0;
					data[4] = 100;
					data[5] = 128;
					data[6] = 0;
					SendToCreate( fd, data, 7 );
				}

				/*if( (pos < 60) || (pos > 240) )
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

				if( pos < 140)
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

				if( pos > 170 )
				{
					// trun right
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


	
