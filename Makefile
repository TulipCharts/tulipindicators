CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

CCFLAGS ?= -Wall -Wextra -Wshadow -Wconversion -std=c99 -pedantic -O2 -g

SRCS=$(wildcard indicators/*.c)
SRCS+=$(wildcard utils/*.c)
OBJS=$(SRCS:%.c=%.o)
AMAL=$(SRCS:%.c=%.ca)

.SUFFIXES: .c .o .h .ca

all: libindicators.a sample example1 example2 fuzzer smoke smoke_amal benchmark cli

indicators.h: build.tcl templates/indicators.h templates/indicators.c templates/candles.h templates/candles.c
	tclsh build.tcl

libindicators.a: indicators.h candles.h indicators.c candles.c $(OBJS)
	$(CC) -c $(CCFLAGS) indicators.c -o indicators.o
	$(CC) -c $(CCFLAGS) candles.c -o candles.o
	$(AR) rcu $@ $^ indicators.o candles.o
	$(RANLIB) $@

smoke: smoke.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm
	./$@

smoke_amal: smoke.o tiamalgamation.o
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

cli: cli.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm

#Benchmark and further testing.
benchmark: benchmark.o libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -lm

#Optional benchmark program. Requires TA-Lib.
benchmark_talib: benchmark.c libindicators.a
	$(CC) $(CCFLAGS) -o $@ $^ -DTA_LIB -lta_lib -lm

#This will build all of Tulip Indicators into one .c file.
#This is recommended to be used with other bindings for ease of build.
tiamalgamation.c: $(AMAL) indicators.ca indicators.h candles.ca candles.h
	cat templates/amal.txt indicators.h candles.h utils/buffer.h utils/localbuffer.h utils/minmax.h $(AMAL) indicators.ca candles.ca > $@


$(OBJS): indicators.h candles.h

smoke.o: indicators.h candles.h

example1.o: indicators.h

example2.o: indicators.h

fuzzer.o: indicators.h candles.h

sample.o: indicators.h candles.h

cli.o: indicators.h candles.h

benchmark.o: indicators.h candles.h

$(AMAL): indicators.h candles.h

.c.o:
	$(CC) -c $(CCFLAGS) $< -o $@

.c.ca:
	$(CC) -E -P $(CCFLAGS) $< -o $@ -D TI_SKIP_SYSTEM_HEADERS -D __TI_INDICATORS_H__ -D __TC_CANDLES_H__ -D __BUFFER_H__ -D __LOCALBUFFER_H__ -D __MINMAX_H__


clean:
	rm -f *.a
	rm -f *.exe
	rm -f *.o
	rm -f indicators/*.o
	rm -f utils/*.o
	rm -f *.ca
	rm -f indicators/*.ca
	rm -f utils/*.ca

veryclean:
	rm -f *.a
	rm -f *.exe
	rm -f *.o
	rm -f indicators/*.o
	rm -f utils/*.o
	rm -f *.ca
	rm -f indicators/*.ca
	rm -f utils/*.ca
	rm -f indicators.h
	rm -f indicators.c
	rm -f candles.h
	rm -f candles.c
	rm -f tiamalgamation.c
	rm -f -r docs

