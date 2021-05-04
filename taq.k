/wget https://kparc.io/EQY_US_ALL_TRADE_20201210.zip ~5gib

csv:"libcsv.so"5:`csv
FILE:"EQY_US_ALL_TRADE_20201210"
SPEC:"|QsssifsgijssQQb";CPU:4;SKIP:0
t:csv(FILE;SPEC;;CPU;SKIP)
-5#t

\\
