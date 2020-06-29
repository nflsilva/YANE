SDIR=src
IDIR=include
TDIR=tests
BDIR=bin
ODIR=obj
TODIR=tobj
LDIR=lib


LIBS=-lstdc++
ANAME=yane
TNAME=tests

CC=g++
CFLAGS=-g -Wall -ansi -std=gnu++11 -I$(IDIR)





_APP_DEPS = *.hpp 
APP_DEPS = $(patsubst %,$(IDIR)/%,$(_APP_DEPS))

_APP_OBJ = r6502.o cartridge_header.o cartridge.o cpu_bus.o mapper000.o ram.o u2c02.o ppu_bus.o nes_console.o
APP_OBJ = $(patsubst %,$(ODIR)/%,$(_APP_OBJ))

_TST_OBJ = nes_test.o main.o
TST_OBJ = $(patsubst %,$(TODIR)/%,$(_TST_OBJ))


$(ODIR)/%.o: $(SDIR)/%.cpp $(APP_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TODIR)/%.o: $(TDIR)/%.cpp $(APP_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


all: $(APP_OBJ)
	$(CC) -c -o $(ODIR)/main.o $(SDIR)/main.cpp $(CFLAGS)
	$(CC) -o $(BDIR)/$(ANAME) $(ODIR)/main.o $^ $(CFLAGS) $(LIBS)

tests: $(TST_OBJ) $(APP_OBJ)
	$(CC) -o $(BDIR)/$(TNAME) $^ $(CFLAGS) $(LIBS)

rtests:
	./$(BDIR)/$(TNAME)
	

.PHONY: clean
	
time: all 
	time ./$(BDIR)/$(ANAME)
	
tclean:
	rm -f $(TODIR)/*.o $(BDIR)/* *.exe

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*.o *~ $(IDIR)/*~ $(SDIR)/*~ $(BDIR)/* *.exe
