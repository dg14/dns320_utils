.PHONY = all
.DEFAULT_GOAL = all
CC?=gcc
CFLAGS?=-I. -I../src -Isrc
LIBS= -lpthread
DEPS = dns320l.h
#OBJ = src/serial.o src/ini.o src/str.o src/socket.o src/proto.o src/debug.o src/log.o src/handlers.o
OBJ = src/serial.o src/ini.o src/str.o src/socket.o src/funs.o src/debug.o src/log.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%: %.o $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

dns320l-daemon: $(OBJ) src/main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

dns320l-cli: $(OBJ) src/cli.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

tests: test/test_str test/test_ini test/test_sockets test/test_leds test/test_log test/test_debug test/test_readserial

test/test_str: $(OBJ) test/test_str.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test/test_ini: $(OBJ) test/test_ini.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test/test_sockets: $(OBJ) test/test_sockets.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test/test_leds: $(OBJ) test/test_leds.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test/test_log: $(OBJ) test/test_log.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test/test_debug: $(OBJ) test/test_debug.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

test/test_readserial: $(OBJ) test/test_readserial.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: dns320l-cli tests

clean:
	rm -f src/*.o test/*.o dns320l-cli dns320l-daemon

PHONY:
