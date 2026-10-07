CC = gcc
CFLAGS = `pkg-config --cflags ibus-1.0` -Wall -g
LIBS = `pkg-config --libs ibus-1.0`
TARGET = ibus-engine-mongol

all: $(TARGET)

$(TARGET): main.c engine.c
	$(CC) $(CFLAGS) -o$(TARGET) main.c engine.c $(LIBS)

clean:
	rm -f $(TARGET)