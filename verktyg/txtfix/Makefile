TARGET = txtfix
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: all clean

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	    $(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	    $(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	    -rm -f *.o
	    -rm -f $(TARGET)
