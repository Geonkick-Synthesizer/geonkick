CC = gcc
INCLUDES = 
#CFLAGS = -O2 -Wall $(INCLUDES)
CFLAGS = -O0 -Wall $(INCLUDES)
LIBS = -ljack -lm
TARGET = libgeonkick.a
SRC = src

OBJECTS = envelope.o \
          oscillator.o \
          gkick_jack.o \
          geonkick.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	ar -cvq $(TARGET) *.o
%.o: $(SRC)/%.c
	$(CC) -c $(CFLAGS)$<

install:
	cp libgeonkick.a /usr/local/lib/
	cp $(SRC)/geonkick.h /usr/local/include/

uninstall:
	rm -fr /usr/local/lib/libgeonkick.a
	rm -fr /usr/local/include/geonkick.h

clean:
	rm -rf *.o $(TARGET)

