ROOT=..
IDIR=$(ROOT)/include
CC=gcc
CFLAGS=-I $(IDIR) -g

ODIR=$(ROOT)/obj
LDIR=$(ROOT)/lib

LIBS=-lm

_DEPS =
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

TARGET=$(ROOT)/bin/wick6774sockserv

$(TARGET): $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
		rm -f $(TARGET) $(ODIR)/*.o *~ core $(INCDIR)/*~
