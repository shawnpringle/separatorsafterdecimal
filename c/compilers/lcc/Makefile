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
	$(LINKER) sdn.def -dll -o sdn.dll $(OBJS) $(LIBS)

# Build sdn.c
SDN_C=\

sdn.obj: $(SDN_C) ..\..\sdn.c
	$(CC) -c $(CFLAGS) ..\..\sdn.c

# Build sdnapi.c

sdnapi.obj: ..\..\sdnapi.c
	$(CC) -c $(CFLAGS) ..\..\sdnapi.c

link:
	$(LINKER) $(SRCDIR)\lcc1\sdn.def  -dll  -o $(SRCDIR)\lcc1\sdn.dll $(OBJS) $(LIBS)

clean:
	del $(OBJS) sdn.dll
