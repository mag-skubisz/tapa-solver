.PHONY: all clean

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
HEADERS = $(wildcard *.h)
CFLAGS += -Wno-pointer-compare

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) -o TapaSolver $(OBJS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f TapaSolver *.o