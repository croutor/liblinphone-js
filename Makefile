# Source, Executable, Includes, Library Defines
INCL   = notifications.h
SRC    = linphone.c notifications.c
OBJ    = $(SRC:.c=.o)
LIBS   = -llinphone -lmediastreamer_base
EXE    = linphonecallsample

# Compiler, Linker Defines
CC      = /usr/bin/gcc
CFLAGS  = -Wall -O2
LIBPATH = -L.
LDFLAGS = -o $(EXE) $(LIBPATH) $(LIBS)
CFDEBUG = -ansi -pedantic -Wall -g -DDEBUG $(LDFLAGS)
RM      = /bin/rm -f

# Compile and Assemble C Source Files into Object Files
%.o: %.c
			$(CC) -c $(CFLAGS) $*.c

# Link all Object Files with external Libraries into Binaries
$(EXE): $(OBJ)
			$(CC) $(LDFLAGS) $(OBJ)

# Objects depend on these Libraries
$(OBJ): $(INCL)

# Create a gdb/dbx Capable Executable with DEBUG flags turned on
debug:
			 $(CC) $(CFDEBUG) $(SRC)

# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
			 $(RM) $(OBJ) $(EXE) core a.out


run:
	cp linphone.conf /tmp/linphone.conf
	./$(EXE)
