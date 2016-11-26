#!/bin/bash
rm server
rm client

rm client_*
rm server_fifo

gcc -g -Wall -o server server.c -lncurses
gcc -g -Wall -o client client.c -lncurses
