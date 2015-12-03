CC=gcc
CFLAGS=-c -Wall
MFLAGS=-lpthread -D_REENTRANT -lm

all: GroupCProject clean

GroupCProject: iRobot.o
	$(CC) iRobot.o -o ./bin/iRobot $(MFLAGS)

iRobot.o: ./src/iRobot.c
	$(CC) $(CFLAGS) ./src/iRobot.c

clean:
	-rm ./*.o
