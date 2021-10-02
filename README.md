## kcsv

```
$ wget https://kparc.io/EQY_US_ALL_TRADE_20201210.zip
$ unzip EQY_US_ALL_TRADE_20201210.zip
$ make

Darwin prng.local 21.1.0 Darwin Kernel Version 21.1.0: Sat Sep 11 12:27:45 PDT 2021; root:xnu-8019.40.67.171.4~1/RELEASE_ARM64_T8101 arm64

fname=EQY_US_ALL_TRADE_20201210 fmt=QsssifsgijssQQb dir=taq ncol=15 ncpu=4 skp=0 sep=| fd=3
seed alloc r->bn 150000128 r->i 0x12af10000
underflow bn 150000128    base = 26428083   water = 150000000  BPF = 5.675785   fix=701367685
underflow bn 851367813    base = 147632028   water = 150000000  BPF = 5.766823   fix=13655677
underflow bn 865023490    base = 149845958   water = 150000000  BPF = 5.772752   fix=889247
underflow bn 865912737    base = 149474803   water = 150000000  BPF = 5.793035   fix=3042485
underflow bn 868955222    base = 149965974   water = 150000000  BPF = 5.794349   fix=197159
underflow bn 869152381    base = 149997531   water = 150000000  BPF = 5.794445   fix=14307
underflow bn 869166688    base = 149999849   water = 150000000  BPF = 5.794450   fix=875
underflow bn 828937207    base = 143479371   water = 150000000  BPF = 5.777396   fix=37672259
file : EQY_US_ALL_TRADE_20201210     0.00 (4.81)GB    rows: 59565292    cores: 4(8)

wall  : 6.36s
read  : 1.95s
scan  : 0.95s
parse : 3.42s

r/s/p/total : 2.46 5.07 1.41 0.76 GB/s

$ ls -l taq

 ls -l taq
total 11299440
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 cnd
-rw-------  1 kelas  staff   59565308 Oct  2 08:06 crr
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 ptm
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 rpt
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 rtm
-rw-------  1 kelas  staff  238261184 Oct  2 08:06 seq
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 src
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 stp
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 sym
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 tid
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 tme
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 trd
-rw-------  1 kelas  staff  238261184 Oct  2 08:06 vol
-rw-------  1 kelas  staff  476522352 Oct  2 08:06 xch
-rw-------  1 kelas  staff    7445683 Oct  2 08:06 xpt

```


```
csv.c   main loop
tok.c   tokenizer
prs.c   parsers
que.c   queue
fdp.c   fast ieee
thr.c   threads
vec.h   avx2neon
clk.h   bench
```

`//:~`
