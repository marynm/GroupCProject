CC=gcc
CFLAGS=-c -Wall

all: GroupCProject clean

GroupCProject: iRobot.o iRobotCreateDrive.o
	$(CC) iRobot.o -o ./bin/iRobot
	$(CC) iRobotCreateDrive.o -o ./bin/iRobotCreateDrive -lm

iRobot.o: ./src/iRobot.c
	$(CC) $(CFLAGS) ./src/iRobot.c

iRobotCreateDrive.o: ./src/iRobotCreateDrive.c
	$(CC) $(CFLAGS) ./src/iRobotCreateDrive.c

clean:
	-rm ./*.o
