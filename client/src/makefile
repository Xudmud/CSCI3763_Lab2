ROOT=..
IDIR=$(ROOT)/include
CC=gcc
CFLAGS=-I $(IDIR) -g

ODIR=$(ROOT)/obj
LDIR=$(ROOT)/lib

LIBS=-lm

_DEPS = myfuncs.h stringparser.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o myfuncs.o stringparser.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)

TARGET=$(ROOT)/bin/wic6774shell

$(TARGET): $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
		rm -f $(TARGET) $(ODIR)/*.o *~ core $(INCDIR)/*~
