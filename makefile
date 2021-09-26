ARMOPT=-march=armv8-a+crc+crypto
ifneq ($(shell uname -m),arm64)
 ifneq ($(shell uname -m),aarch64)
  A=-march=native
 else
  A=$(ARMOPT)
 endif
else
 A=$(ARMOPT)
endif

# this could be an extra ./configure step
# - require either gcc-10 or clang
ifeq      ($(shell gcc   -v 2>&1|grep -o 'gcc version 10'),gcc version 10)
 CC=gcc
 W=-fmax-errors=1 -flax-vector-conversions
else ifeq ($(shell clang -v 2>&1|grep -o 'clang' ),clang)
 CC=clang
 W=-ferror-limit=1 -Wno-visibility -Wno-shift-op-parentheses
else
 $(error no suitable compiler)
endif
W:=$W -Wno-unused-value -Wno-int-conversion -Wno-parentheses -w


C=$(CC) -O3 -g $W

all: test

so: *.c *.h
	$C -olibcsv.so ???.c -lm $A -lpthread -shared -fPIC # -undefined dynamic_lookup
	ls -la libcsv.so

test: so makefile *.c *.h
	$C -ocsv main.c -L. -lcsv $A
	LD_LIBRARY_PATH=. ./csv EQY_US_ALL_TRADE_20201210 "|QsssifsgijssQQb" taq

#:~
