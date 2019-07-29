# Wedit Makefile for project sdn
SRCDIR=..
CFLAGS=-g2 
CC=$(LCCROOT)\bin\lcc.exe
LINKER=$(LCCROOT)\bin\lcclnk.exe
OBJS=\
sdnapi.obj \

LIBS=
EXE=sdn.dll

$(EXE):$(OBJS) Makefile
	$(LINKER) sdn.def -dll -nounderscores -o sdn.dll $(OBJS) $(LIBS)


# Build sdnapi.c
sdnapi.obj: ..\..\sdnapi.c ..\..\sdn.h
	$(CC) -c $(CFLAGS) ..\..\sdnapi.c

paxdiablo.obj:  ..\..\paxdiablo.c ..\..\sdn.h
	$(CC) -c $(CFLAGS) -I..\.. ..\..\paxdiablo.c 
	
sdntest: paxdiablo.obj sdnapi.obj
	$(LINKER) -o sdntest.exe sdnapi.obj paxdiablo.obj
	
clean:
	del $(OBJS) sdn.dll paxdiablo.obj sdntest.exe
