CC = gcc
INCLUDES =
CFLAGS += -O0 -g -Wall -fPIC $(DEFINES) $(INCLUDES)
TARGET = libgeonkick.a
SRC = src

OBJECTS = gkick_log.o \
          envelope.o \
          gkick_buffer.o \
          filter.o \
          compressor.o \
          distortion.o \
          oscillator.o \
          synthesizer.o \
          gkick_jack.o \
          audio_output.o \
          gkick_audio.o \
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

