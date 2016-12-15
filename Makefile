CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

#CCFLAGS = -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pg -fprofile-arcs -ftest-coverage
#CCFLAGS = -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pedantic -std=c99
CCFLAGS = -Wall -Wextra -Wshadow -Wconversion -O2 -g

SRCS=$(wildcard indicators/*.c)
SRCS+=$(wildcard utils/*.c)
OBJS=$(SRCS:%.c=%.o)


all: libindicators.a sample example1 example2 fuzzer smoke

libindicators.a: indicators_index.o $(OBJS)
	$(AR) rcu $@ $^
	$(RANLIB) $@


indicators_index.o: indicators.h


smoke: smoke.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm
	./$@

example1: example1.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm

example2: example2.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm

fuzzer: fuzzer.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm

benchmark: benchmark.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lta_lib -lm

sample: sample.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@


clean:
	rm -f *.a
	rm -f *.exe
	rm -f *.o
	rm -f indicators/*.o
	rm -f utils/*.o
	#rm -f -r docs
