MAKEFLAGS = --no-print-directory

CC = gcc
DB = lldb

CFLAGS = -Wall -Wextra
LDFLAGS =

DBFLAGS = -Og -g
RLFLAGS = -O3 -march=native -mtune=native -DNDEBUG

FILES = $(wildcard *.c)
OBJECTS = $(FILES:.c=.o)
TARGET = main

debug:
	@make clean
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) $(FILES)

release:
	@make clean
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(RLFLAGS) $(FILES)

clean:
	@rm -rf $(TARGET) *.exe $(OBJECTS)

run:
	@make debug
	@echo "-------------------------"
	@./$(TARGET)
	@echo "-------------------------"
	@make clean

rundebug:
	@make debug
	$(DB) ./$(TARGET)
	@make clean
