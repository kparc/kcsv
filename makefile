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

LVM=clang
GNU=gcc-10

W=-Wno-shift-op-parentheses -Wno-unused-value -Wno-int-conversion -Wno-visibility -Wno-parentheses -w

C=$(LVM) -O3 -g -ferror-limit=1 $W

all: test

so: *.c *.h
	$C -olibcsv.so ???.c -lm $A -lpthread -shared -fPIC # -undefined dynamic_lookup
	ls -la libcsv.so

test: so makefile *.c *.h
	$C -ocsv main.c -L. -lcsv $A
	LD_LIBRARY_PATH=. ./csv EQY_US_ALL_TRADE_20201210 "|QsssifsgijssQQb" taq

#:~
