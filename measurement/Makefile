OBJECTS = $(wildcard ../rand/*.o ../hash/*.o ../*.o)
EXECUTABLES = $(patsubst %.c, %, $(wildcard *.c))

all : $(EXECUTABLES)

% : %.c
	$(CC) $(CFLAGS) $(PFLAGS) -o  $@ $(OBJECTS) $@.c $(LDLIBS)

clean :
	rm -f $(EXECUTABLES)
	rm -rf analysis


.PHONY : all clean
