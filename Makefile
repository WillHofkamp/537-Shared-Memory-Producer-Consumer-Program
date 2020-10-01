#
# Sample makefile updated to include the commands to run the Clang Static Analyzer
#
#    scan-build will "make" your program and run CSA at the same time.
#
#    scan-view will do a scan-build (if necessary) and then open a web browser
#      window to review the results.
#
CC = gcc
WARNING_FLAGS = -Wall -Wextra
EXE = prodcomm
SCAN_BUILD_DIR = scan-build-out

all: main.o queue.o reader.o munch1.o munch2.o writer.o
	$(CC) -o $(EXE) main.o queue.o reader.o munch1.o munch2.o writer.o -lpthread
main.o: main.c queue.h reader.h munch1.h munch2.h writer.h
	$(CC) $(WARNING_FLAGS) -c main.c -lpthread
quote.o: queue.c queue.h
	$(CC) $(WARNING_FLAGS) -c queue.c
reader.o: reader.c reader.h
	$(CC) $(WARNING_FLAGS) -c reader.c
munch1.o: munch1.c munch1.h
	$(CC) $(WARNING_FLAGS) -c munch1.c
munch2.o: munch2.c munch2.h
	$(CC) $(WARNING_FLAGS) -c munch2.c
writer.o: writer.c writer.h
	$(CC) $(WARNING_FLAGS) -c writer.c
clean:
	rm -f $(EXE) *.o
	rm -rf $(SCAN_BUILD_DIR)

#
# Run the Clang Static Analyzer
#
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

#
# View the one scan available using firefox
#
scan-view: scan-build
	firefox -new-window $(SCAN_BUILD_DIR)/*/index.html 
