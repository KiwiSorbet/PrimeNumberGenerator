CC = clang
DEBUGFLAGS = -O0 -g -Wall -lm
BUILDFLAGS = -O3 -lm
BUILDFILES = main.c bmap.c

release:
	@make clean --no-print-directory
	$(CC) $(BUILDFLAGS) $(BUILDFILES) -o main
	
debug:
	@make clean --no-print-directory
	$(CC) $(DEBUGFLAGS) $(BUILDFILES) -o main

clean:
	rm -f main *.exe *.o

run:
	@make debug --no-print-directory
	./main
	@make clean --no-print-directory