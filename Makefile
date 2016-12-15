CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

#CCFLAGS = -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pg -fprofile-arcs -ftest-coverage -lm
#CCFLAGS = -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pedantic -std=c99 -lm
CCFLAGS = -Wall -Wextra -Wshadow -Wconversion -O2 -g -lm

SRCS=$(wildcard indicators/*.c)
SRCS+=$(wildcard utils/*.c)
OBJS=$(SRCS:%.c=%.o)


all: libindicators.a sample example1 example2 fuzzer smoke

libindicators.a: indicators_index.o $(OBJS)
	$(AR) rcu $@ $^
	$(RANLIB) $@


indicators_index.o: indicators.h


smoke: smoke.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^
	./$@

example1: example1.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^

example2: example2.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^

fuzzer: fuzzer.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^

benchmark: benchmark.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lta_lib

sample: sample.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^

tulipind.dll: interface.c libindicators.a interface.def
	$(CC) $(CCFLAGS) -o $@ $^ -shared

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@


clean:
	rm -f *.a
	rm -f *.exe
	rm -f *.o
	rm -f indicators/*.o
	rm -f utils/*.o
	#rm -f -r docs
