#include"k.h" //!< a (very) quick and dirty standalone smoke test
K csv(K x);   //!< x:(infile;format;colnames;outdir;maxcore;skip)
int main(I c,char**a){QQ(c-4,"file|fifo fmt dir")K r=tn(0,6);r=(K[6]){Sc(a[1]),Sc(a[2]),0,Sc(a[3]),NCPU/2,0};csv(r);}
//:~
