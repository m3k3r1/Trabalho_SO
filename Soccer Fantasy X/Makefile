CC=gcc
#
IDIR=include
CFLAGS=-I$(IDIR) -Wall -g -lncurses -lpthread

ODIR=obj
SRC=src

_DEPS = base.h usr_mgmt.h usr_ui.h thd_mgmt.h ses_mgmt.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = server.o client.o usr_mgmt.o usr_ui.o thd_mgmt.o ses_mgmt.o
OBJ= $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: server client

server : $(OBJ)
	$(CC) -o  $@ $(SRC)/server.c $(SRC)/usr_mgmt.c $(SRC)/usr_ui.c $(SRC)/thd_mgmt.c $(SRC)/ses_mgmt.c $(CFLAGS)

client : $(OBJ)
	$(CC) -o  $@ $(SRC)/client.c $(SRC)/usr_mgmt.c $(SRC)/usr_ui.c $(SRC)/thd_mgmt.c $(SRC)/ses_mgmt.c $(CFLAGS)

clean:
	rm server client obj/*

unlink:
	rm res/client_*
	rm res/server_fifo
