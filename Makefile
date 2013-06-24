TARGET = bin
LIBS = -lm
CC = gcc
CFLAGS = -g

.PHONY:  clean default

default: clean compile run

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

compile: $(OBJECTS)
	mkdir $(TARGET)
	$(CC) $(OBJECTS)  $(LIBS) -o $(TARGET)/specie

clean:
	-rm src/*.o
	-rm -fr $(TARGET)

test: default
	-valgrind   --leak-check=full --leak-check=yes --track-origins=yes  $(TARGET)/specie

run:
	./$(TARGET)/specie
