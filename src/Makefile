SERVER_SOURCES := $(wildcard server/*.c)
SERVER_EXECS := $(SERVER_SOURCES:.c=.exe)

CLIENT_SOURCES := $(wildcard client/*.c)
CLIENT_EXECS := $(CLIENT_SOURCES:.c=.exe)

all: $(SERVER_EXECS) $(CLIENT_EXECS)

server: $(SERVER_EXECS)

client: $(CLIENT_EXECS)

%.exe: %.c
	gcc -o $@ $< -lws2_32