CC ?= gcc
CXX ?= g++
AR ?= ar
RANLIB ?= ranlib
LD=g++

# CCFLAGS ?= -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pg -fprofile-arcs -ftest-coverage
# CCFLAGS ?= -Wall -Wextra -Werror -Wshadow -Wconversion -O2 -g -pedantic -std=c99
CCFLAGS ?= -Wall -Wextra -Wshadow -Wconversion -Wno-write-strings -O2 -g
CXXFLAGS ?= -Wall -Wextra -Wshadow -Wno-conversion -Wno-write-strings -O2 -g --std=c++14

SRCS_UTILS=$(wildcard utils/*.c)
SRCS_INDICATORS=$(wildcard indicators/*.c)
SRCS_PST_INDICATORS=$(wildcard pst_indicators/*.cpp)

SRCS=$(SRCS_UTILS)
SRCS+=$(SRCS_INDICATORS)
SRCS+=$(SRCS_PST_INDICATORS)

OBJ_UTILS=$(SRCS_UTILS:%.c=%.o)
OBJ_INDICATORS=$(SRCS_INDICATORS:%.c=%.o)
OBJ_PST_INDICATORS=$(SRCS_PST_INDICATORS:%.cpp=%.o)

OBJS=$(SRCS:%.c=%.o)
OBJS+=$(SRCS:%.cpp=%.o)

# AMAL=$(SRCS:%.c=%.ca)
AMAL=$(SRCS_UTILS:%.c=%.ca)
AMAL+=$(SRCS_INDICATORS:%.c=%.ca)


.SUFFIXES: .c .o .h .ca

.PHONY: all
all: libindicators.a sample example1 example2 fuzzer smoke 
libtiamal: libtiamal.a smoke_tiamal
libindicators: libindicators.so smoke_so sample_so

### libindicators.so ###########################################################
smoke_so: smoke.o utils/buffer.o libindicators.so
	# $(LD) -lm smoke.o utils/buffer.o -L. -lindicators -o $@ 
	$(LD) -lm smoke.o utils/buffer.o -L. -lindicators -Wl,-rpath=. -o $@ 
	./$@

sample_so: sample.o sample_pst.o libindicators.so
	$(LD) -lm sample.o sample_pst.o -L. -lindicators -Wl,-rpath=. -o $@ 
	./$@ rsi 5

libindicators.so: indicators_index.o $(OBJ_UTILS) $(OBJ_INDICATORS) $(OBJ_PST_INDICATORS)
	$(LD) -lm $^ -o $@ -shared

### tiamal.a ###################################################################
smoke_tiamal: smoke.o libtiamal.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm
	./$@

libtiamal.a: tiamalgamation.o $(OBJ_PST_INDICATORS)
	$(AR) rcu $@ $^
	$(RANLIB) $@

tiamalgamation.o: tiamalgamation.c

#This will build all of Tulip Indicators into one .c file.
tiamalgamation.c: $(AMAL) indicators_index.ca indicators.h
	echo -e "/*\n * TULIP INDICATORS AMALGAMATION\n * This is all of Tulip Indicators in one file.\n * To get the original sources, go to https://tulipindicators.org\n */\n\n" \
	    | cat - indicators.h utils/buffer.h utils/minmax.h $(AMAL) indicators_index.ca > $@


### libindicators.a ############################################################
libindicators.a: indicators_index.o $(OBJS)
	$(AR) rcu $@ $^
	$(RANLIB) $@


indicators_index.o: indicators.h


smoke: smoke.o utils/buffer.o libindicators.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm
	./$@

example1: example1.o libindicators.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

example2: example2.o libindicators.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

fuzzer: fuzzer.o libindicators.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

sample: sample.o sample_pst.o libindicators.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

#Optional benchmark program. Requires TA-Lib.
benchmark: benchmark.o libindicators.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -lta_lib -lm

.h.c .h.cpp:

.c.o .cpp.o:
	$(CXX) -c $(CXXFLAGS) $< -o $@ -fPIC

.c.ca:
	$(CXX) -E -P $(CXXFLAGS) $< -o $@ -D TI_SKIP_SYSTEM_HEADERS -D __TI_INDICATORS_H__ -D __BUFFER_H__ -D __MINMAX_H__


.PHONY: clean
clean:
	rm -f *.o
	rm -f *.ca
	rm -f *.a
	rm -f *.exe
	rm -f tiamalgamation.c
	rm -f utils/*.o
	rm -f utils/*.ca
	rm -f indicators/*.o
	rm -f indicators/*.ca
	rm -f pst_indicators/*.o
	rm -f pst_indicators/*.ca
	#rm -f -r docs


.PHONY: release
release:
	mkdir -p bin
	mv -f libindicators.a sample example1 example2 fuzzer smoke bin
	mv -f *.o bin
