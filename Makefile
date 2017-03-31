CC = gcc
INCLUDES = 
CFLAGS = -O2 -Wall $(INCLUDES)
LIBS = -ljack -lm
TARGET = geonkick
SRC = ./

OBJECTS = main.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ $(LIBS) -o $(TARGET)
%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS)$<

clean:
	rm -rf *.o $(TARGET)

