#find sources in the current directory and objects based in their names
SOURCES := $(shell find . -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp, %.o, ${SOURCES})

#compiler settings
CC = g++

#compiler flags
override CFLAGS += -g -O2 -Wall

TARGET = golay

#target build
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)


