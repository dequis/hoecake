CC=gcc
CFLAGS=-fPIC -Wall
LDFLAGS=-ldl -shared
LD_PRELOAD=$(abspath ldpreload.so)

all: ldpreload.so

%.so: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean test
clean:
	rm *.so
