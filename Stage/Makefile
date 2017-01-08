CC=gcc
#
IDIR=include
CFLAGS=-I$(IDIR) -Wall -g -lncurses -lpthread

ODIR=obj
SRC=src

_DEPS = base.h usr_mgmt.h game_mgmt.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o client.o usr_mgmt.o base.o game_mgmt.o
OBJ= $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: server client

server : $(OBJ)
	$(CC) -o  $@ $(SRC)/server.c $(SRC)/usr_mgmt.c $(SRC)/base.c $(SRC)/game_mgmt.c $(CFLAGS)

client : $(OBJ)
	$(CC) -o  $@ $(SRC)/client.c $(SRC)/usr_mgmt.c $(SRC)/base.c $(SRC)/game_mgmt.c $(CFLAGS) 

clean:
	rm server client obj/*

unlink:
	rm res/client_* res/server_fifo
