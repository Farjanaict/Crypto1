CC = gcc
CFLAGS = -Wall -Wextra -std=c11
ifeq ($(OS),Windows_NT)
    SOCKET_LIB = -lws2_32
    EXE = .exe
else
    SOCKET_LIB =
    EXE =
endif

all: server$(EXE) client$(EXE)

server$(EXE): src/server.c src/layers.c src/layers.h
	$(CC) $(CFLAGS) src/server.c src/layers.c -o server$(EXE) $(SOCKET_LIB)

client$(EXE): src/client.c src/layers.c src/layers.h
	$(CC) $(CFLAGS) src/client.c src/layers.c -o client$(EXE) $(SOCKET_LIB)

clean:
	rm -f server server.exe client client.exe
