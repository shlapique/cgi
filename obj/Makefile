CC=g++
CFLAGS=-Wall
OBJS=main.o Math.o
LINKER_FLAGS=-lSDL2
TARGET=out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LINKER_FLAGS) -o $(TARGET) main.o Math.o 

main.o: main.cpp Math.h Core.h
	$(CC) $(CFLAGS)  -c main.cpp

Math.o: Math.h Core.h

clean:
	rm -rf $(OBJS) $(TARGET)
