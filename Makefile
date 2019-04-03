CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

CCFLAGS ?= -Wall -Wextra -Wshadow -Wconversion -std=c99 -pedantic -O2 -g

SRCS=$(wildcard indicators/*.c)
SRCS+=$(wildcard utils/*.c)
OBJS=$(SRCS:%.c=%.o)
AMAL=$(SRCS:%.c=%.ca)

.SUFFIXES: .c .o .h .ca

all: libindicators.a sample example1 example2 fuzzer smoke

indicators.h: indicators.tcl
	tclsh indicators.tcl

libindicators.a: indicators.h $(OBJS)
	$(CC) -c $(CCFLAGS) indicators_index.c -o indicators_index.o
	$(AR) rcu $@ $^ indicators_index.o
	$(RANLIB) $@

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


$(OBJS): indicators.h

smoke.o: indicators.h

example1.o: indicators.h

example2.o: indicators.h

fuzzer.o: indicators.h

sample.o: indicators.h

benchmark.o: indicators.h

$(AMAL): indicators.h

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

.c.ca:
	$(CC) -E -P $(CCFLAGS) $< -o $@ -D TI_SKIP_SYSTEM_HEADERS -D __TI_INDICATORS_H__ -D __BUFFER_H__ -D __MINMAX_H__


clean:
	rm -f *.a
	rm -f *.exe
	rm -f *.o
	rm -f indicators.h
	rm -f indicators_index.c
	rm -f indicators/*.o
	rm -f utils/*.o
	rm -f *.ca
	rm -f indicators/*.ca
	rm -f utils/*.ca
	rm -f tiamalgamation.c
	#rm -f -r docs
