# Compiler
CC = gcc
# Optimized, Warning,
CFLAGS = -O3 -Wall -Wpedantic -Wformat -Wformat-security -Wunreachable-code -p
# Profiling flag
PFLAGS = -pg
# Linking Crypto Library
LDLIBS = -lcrypto -lssl
# No echoing
MAKEOPTION = --silent

export CC
export CFLAGS
export PFLAGS
export LDLIBS
export MAKEOPTION

TARGET = sds
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h) $(wildcard rand/*.h) $(wildcard hash/*.h)
TESTS = $(patsubst tests/%.c, tests/%, $(wildcard tests/*.c))
MEASUREMENTS = $(patsubst measurement/%.c, %, $(wildcard measurement/*.c))



all : $(OBJECTS)
# Calling ./random/Makefile
	+$(MAKE) $(MAKEOPTION) -C rand
# Calling ./hash/Makefile
	+$(MAKE) $(MAKEOPTION) -C hash

%.o :  %.c $(HEADERS)
# Compiling files in current directory
	$(CC) $(CFLAGS) $(PFLAGS) -o $@ -c $< $(LDLIBS)

compile_tests : all
# Calling ./tests/Makefile
	+$(MAKE) $(MAKEOPTION) -C tests

run_tests : compile_tests
# Running tests
	@for i in $(TESTS) ; do \
		$$i ; \
	done
	+$(MAKE) $(MAKEOPTION) clean -C tests

compile_measurements : all
#Calling ./measurement/Makefile
	+$(MAKE) $(MAKEOPTION) -C measurement

run_measurements : compile_measurements
# Running measurements
	@cd measurement && \
	mkdir analysis && \
	for i in $(MEASUREMENTS) ; do \
		./$$i && \
		gprof $$i gmon.out > analysis/$$i.txt &&\
		rm -f gmon.out; \
	done
	+$(MAKE) $(MAKEOPTION) clean -C measurement

clean :
	+$(MAKE) $(MAKEOPTION) clean -C rand
	+$(MAKE) $(MAKEOPTION) clean -C hash
	+$(MAKE) $(MAKEOPTION) clean -C tests
	+$(MAKE) $(MAKEOPTION) clean -C measurement
	rm -f *.o

.PHONY : all $(TARGET) run_tests run_measurements clean
