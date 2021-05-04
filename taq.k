/wget https://kparc.io/EQY_US_ALL_TRADE_20201210.zip ~5gib

csv:"libcsv.so"5:`csv
FILE:"EQY_US_ALL_TRADE_20201210"
COLS:`tme`xch`sym`cnd`vol`trd`stp`crr`seq`tid`src`rpt`ptm`rtm`xpt
SPEC:"|QsssifsgijssQQb";CPU:4;SKIP:0

\t t:csv(FILE;SPEC;COLS;CPU;SKIP)

5#t
-5#t
#t

\\
