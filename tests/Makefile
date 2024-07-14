OBJECTS = $(wildcard ../rand/*.o ../hash/*.o ../*.o)
EXECUTABLES = $(patsubst %.c, %, $(wildcard *.c))

all : $(EXECUTABLES)

% : %.c
	$(CC) $(CFLAGS) -o  $@ $(OBJECTS) $@.c $(LDLIBS)

clean :
	rm -f $(EXECUTABLES)


.PHONY : all clean
