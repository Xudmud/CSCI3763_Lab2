#Compiler and flags
CC=gcc



#Libraries
LIBS=-lm

#Server
SERVROOT=server
SERVIDIR=$(SERVROOT)/inc
SERVODIR=$(SERVROOT)/obj
SERVSRCDIR=$(SERVROOT)/src
_SERVDEPS = catalog.h download.h spwd.h upload.h functions.h calendar.h cat.h
SERVDEPS = $(patsubst %,$(SERVIDIR)/%,$(_SERVDEPS))
_SERVOBJ = main.o catalog.o download.o spwd.o upload.o functions.o calendar.o cat.o
SERVOBJ = $(patsubst %,$(SERVODIR)/%,$(_SERVOBJ))
SERVCFLAGS=-I $(SERVIDIR) -g
$(SERVODIR)/%.o: $(SERVSRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(SERVCFLAGS)
SERVTARGET=wic6774sockserver

#Client
CLIROOT=client
CLIIDIR=$(CLIROOT)/inc
CLIODIR=$(CLIROOT)/obj
CLISRCDIR=$(CLIROOT)/src
_CLIDEPS = catalog.h download.h ls.h pwd.h spwd.h upload.h functions.h calendar.h cat.h
CLIDEPS = $(patsubst %,$(CLIIDIR)/%,$(_CLIDEPS))
_CLIOBJ = main.o catalog.o download.o ls.o pwd.o spwd.o upload.o functions.o calendar.o cat.o
CLIOBJ = $(patsubst %,$(CLIODIR)/%,$(_CLIOBJ))
CLICFLAGS=-I $(CLIIDIR) -g
$(CLIODIR)/%.o: $(CLISRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CLICFLAGS)
CLITARGET=wic6774sockclient

all: $(SERVTARGET) $(CLITARGET)

wic6774sockserver: $(SERVOBJ)
	$(CC) -o $@ $^ $(SERVCFLAGS) $(LIBS)
wic6774sockclient: $(CLIOBJ)
	$(CC) -o $@ $^ $(CLICFLAGS) $(LIBS)

#Prevent a file called "clean" from being removed by clean.
#Unlikely, but just in case
.PHONY: clean

clean:
	rm -f $(SERVTARGET) $(CLITARGET) $(SERVODIR)/*.o $(CLIODIR)/*.o *~ core $(INCDIR)/*~
