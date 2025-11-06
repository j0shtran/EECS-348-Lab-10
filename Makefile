CC = g++
CFLAGS = -Wall 

all: lab10

lab10: lab10.cpp 
	$(CC) $(CFLAGS) lab10.cpp -o lab10

clean: 
	rm rf *.o -f lab10
