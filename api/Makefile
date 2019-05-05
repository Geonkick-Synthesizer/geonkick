CC = gcc
INCLUDES = -DGKICK_LIB_LOG_LEVEL_TRACE -DGKICK_LIB_LOG_FUNCTION
CFLAGS += -O2 -Wall -fPIC -fvisibility=hidden $(DEFINES) $(INCLUDES)
SRC = src
BUILD = .
TARGET = libgeonkick.a

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
	cd $(BUILD) && ar -cvq $(TARGET) *.o

%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD)/$@

install:
	cp libgeonkick.a /usr/local/lib/
	cp $(SRC)/geonkick.h /usr/local/include/

uninstall:
	rm -fr /usr/local/lib/libgeonkick.a
	rm -fr /usr/local/include/geonkick.h

clean:
	rm -rf *.o $(TARGET) $(OBJECTS)

