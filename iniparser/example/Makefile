#
# iniparser tests Makefile
#

CC      ?= gcc
CFLAGS  += -g -I../src
LFLAGS  += -L.. -liniparser
AR	    ?= ar
ARFLAGS += rcv
RM      ?= rm -f


default: all

all: iniexample parse

iniexample: inigenerate.c
	$(CC) $(CFLAGS) -o inigenerate inigenerate.c -I../src -L../lib -liniparser

parse: iniparse.c
	$(CC) $(CFLAGS) -o iniparse iniparse.c -I../src -L../lib -liniparser

clean veryclean:
	$(RM) inigenerate example.ini iniparse



