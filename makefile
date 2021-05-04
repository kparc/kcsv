ifneq ($(shell uname -m),arm64)
 A=-march=native
else
 A=-march=armv8-a+crc+crypto
endif

LVM=clang
GNU=gcc-10

W=-Wno-shift-op-parentheses -Wno-unused-value -Wno-int-conversion -Wno-visibility -Wno-parentheses -w

C=$(LVM) -O3 -g -ferror-limit=1 $W

all: *.c *.h
	rm -f libcsv.so
	$C -olibcsv.so csv.c tok.c thr.c prs.c que.c -lm $A -lpthread -shared -fPIC  -undefined dynamic_lookup
	ls -la libcsv.so
	cp libcsv.so ../k9-2021/
	cd ../k9-2021/ && ./Mi2.0 t/csv.k

#:~
