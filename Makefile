CC=gcc
CFLAGS=-c -Wall

all: GroupCProject clean

GroupCProject: iRobot.o
	$(CC) iRobot.o -o ./bin/iRobot

iRobot.o: ./src/iRobot.c
	$(CC) $(CFLAGS) ./src/iRobot.c

clean:
	-rm ./*.o
