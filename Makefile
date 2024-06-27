MAKEFLAGS = --no-print-directory

CC = clang
DB = lldb

CFLAGS = -Wall -Wextra
LDFLAGS =

DBFLAGS = -O0 -g
RLFLAGS = -O3 -march=native -mtune=native -DNDEBUG

FILES = $(wildcard *.c)
OBJECTS = $(FILES:.c=.o)
TARGET = main

debug:
	@make clean --no-print-directory
	$(CC) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) $(FILES) -o $(TARGET)

release:
	@make clean --no-print-directory
	$(CC) $(CFLAGS) $(LDFLAGS) $(RLFLAGS) $(FILES) -o $(TARGET)
	
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