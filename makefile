#Compiler and flags
CC=gcc
CFLAGS=-I $(IDIR) -g

#Libraries
LIBS=-lm

#Server
SERVROOT=server
SERVIDIR=$(SERVROOT)/inc
SERVODIR=$(SERVROOT)/obj
SERVSRCDIR=$(SERVROOT)/src
_SERVDEPS =
SERVDEPS = $(patsubst %,$(SERVIDIR)/%,$(_SERVDEPS))
_SERVOBJ = main.o
SERVOBJ = $(patsubst %,$(SERVODIR)/%,$(_SERVOBJ))
$(SERVODIR)/%.o: $(SERVSRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
SERVTARGET=wic6774sockserver

#Client
CLIROOT=client
CLIIDIR=$(CLIROOT)/inc
CLIODIR=$(CLIROOT)/obj
CLISRCDIR=$(CLIROOT)/src
_CLIDEPS =
CLIDEPS = $(patsubst %,$(CLIIDIR)/%,$(_CLIDEPS))
_CLIOBJ = main.o
CLIOBJ = $(patsubst %,$(CLIODIR)/%,$(_CLIOBJ))
$(CLIODIR)/%.o: $(CLISRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
CLITARGET=wic6774sockclient

all: $(SERVTARGET) $(CLITARGET)

wic6774sockserver: $(SERVOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
wic6774sockclient: $(CLIOBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Prevent a file called "clean" from being removed by clean.
#Unlikely, but just in case
.PHONY: clean

clean:
	rm -f $(SERVTARGET) $(CLITARGET) $(SERVODIR)/*.o $(CLIODIR)/*.o *~ core $(INCDIR)/*~
