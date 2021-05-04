//! naive benchmark
#include<time.h>
typedef struct prof{F total_wtime;F total_rtime;F total_ttime;F total_ptime;U total_bytes;U total_parsed;U total_rows;I total_cores;}prof;
//                           wall          read      tokenize         parse
#define GiB               (1024*1024*1024)
#define BENCH()           F wall=0;struct timespec start,end;
#define TIME(x)           clock_gettime(CLOCK_REALTIME,&x);
#define MSEC(x)           (x.tv_sec*1000+x.tv_nsec/1000000.0)
#define WALL(a...)        TIME(start);a;TIME(end);wall=(MSEC(end)-MSEC(start))/1000.0;
#define ttl(s)            prf->total_##s
#define GBs(volume,time)  ttl(volume)/ttl(time)/GiB

//:~
