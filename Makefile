CC = gcc
CFLAGS = -W

newshell: major2.o major2_functions.o path.o
	$(CC) $(CFLAGS) -o newshell major2.o major2_functions.o path.o

major2.o: major2.c
	$(CC) $(CFLAGS) -c major2.c

major2_functions.o: major2_functions.c
	$(CC) $(CFLAGS) -c major2_functions.c

path.o: path.c
	$(CC) $(CFLAGS) -c path.c