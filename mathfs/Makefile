CC=gcc
OSXFUSE_CFLAGS=$(shell pkg-config fuse --cflags --libs)
CFLAGS=-g $(OSXFUSE_CFLAGS) -D_FILE_OFFSET_BITS=64 -lm
CFLAGS_O=-g -c $(OSXFUSE_CFLAGS) 
O_FILES=$(patsubst %.c,%.o,$(wildcard *.c))

all: build
 
build: $(O_FILES)
	$(CC) $(O_FILES) $(CFLAGS) -o mathfs
    
clean:
	rm -rf *.o mathfs
