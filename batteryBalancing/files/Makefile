CC = gcc

OBJECTS = cell.o battery.o main.o
SRC = ./src/*.c

INCLUDE = -I./inc/ 
VPATH = ./src/
CFLAGS = -lpthread $(INCLUDE)

pratik: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o program

%.o: %.c
	$(CC) $(CFLAGS) -c $(SRC) 
   
clean:
	rm -f *.o
	rm -f program
