CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

#CCFLAGS ?= -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pg -fprofile-arcs -ftest-coverage
#CCFLAGS ?= -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pedantic -std=c99
CCFLAGS ?= -Wall -Wextra -Wshadow -Wconversion -O2 -g

SRCS=$(wildcard indicators/*.c)
SRCS+=$(wildcard utils/*.c)
OBJS=$(SRCS:%.c=%.o)
AMAL=$(SRCS:%.c=%.ca)

.SUFFIXES: .c .o .h .ca

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

sample: sample.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm

#Optional benchmark program. Requires TA-Lib.
benchmark: benchmark.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lta_lib -lm

#This will build all of Tulip Indicators into one .c file.
tiamalgamation.c: $(AMAL) indicators_index.ca indicators.h
	echo -e "/*\n * TULIP INDICATORS AMALGAMATION\n * This is all of Tulip Indicators in one file.\n * To get the original sources, go to https://tulipindicators.org\n */\n\n" \
	    | cat - indicators.h utils/buffer.h utils/minmax.h $(AMAL) indicators_index.ca > $@

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

.c.ca:
	$(CC) -E -P $(CCFLAGS) $< -o $@ -D TI_SKIP_SYSTEM_HEADERS -D __TI_INDICATORS_H__ -D __BUFFER_H__ -D __MINMAX_H__


clean:
	rm -f *.a
	rm -f *.exe
	rm -f *.o
	rm -f indicators/*.o
	rm -f utils/*.o
	rm -f *.ca
	rm -f indicators/*.ca
	rm -f utils/*.ca
	rm -f tiamalgamation.c
	#rm -f -r docs
