OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

all : $(OBJECTS)

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) $(PFLAGS) -o $@ -c $< $(LDLIBS)

clean :
	rm -f *.o

.PHONY : all clean
