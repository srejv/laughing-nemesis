#
# Sample Makefile for lsh
# lab1 in Operating System course
#
BIN=	lsh_test

SRCS=	parse.c lsh_test.c lsh_shell.c
OBJS=	parse.o lsh_test.o lsh_shell.o

CC=	gcc
CFLAGS= -g 
## Turn on this for more warnings:
#CFLAGS= -g -Wall -pedantic
LIBS= -lreadline -ltermcap

.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

all:	$(BIN)

lsh:	$(OBJS)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJS) $(LIBS)

clean:
	-rm -f $(OBJS) lsh