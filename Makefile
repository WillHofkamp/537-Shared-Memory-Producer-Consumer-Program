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

all: prodcom.o thread.o queue.o
	$(CC) -o $(EXE) prodcom.o thread.o queue.o -lpthread
prodcom.o: prodcom.c queue.h thread.h
	$(CC) $(WARNING_FLAGS) -c prodcom.c -lpthread
thread.o: thread.c thread.h queue.h
	$(CC) $(WARNING_FLAGS) -c thread.c
quote.o: queue.c queue.h
	$(CC) $(WARNING_FLAGS) -c queue.c
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
