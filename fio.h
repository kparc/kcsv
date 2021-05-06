//! splayed table i/o, copyright (c) 2021 regents of kparc, bsd-2-clause

#include"k.h"

I mkd(S s);              //!< mkdir
I rwd(I d,U o);          //!< rewind descriptor d to position o
U wr (I d,S b,U n);      //!< short for write(2) descriptor, buffer, buffer size
U fsz(I d);              //!< file size

_ ffo(S f);              //!< signal if fifo
I dir(K x);              //!< mkdir if doesn't exist, change cwd
I acc(S s,I f);          //!< check access to file s, f=0|1 write|read
K map(S f,U*n,I d);      //!< mmap filename f or descriptor d as K column and close d
I opr(K x);              //!< open file x for reading, or create a fifo if x doesn't exist
I*opw(K x,K y);          //!< check|create output dir x, create|truncate splayed columns from y, return file descriptors
K cork(I*fds,K r,U rct); //!< update metablocks of splayed column descriptors fds of length rct, re-populate r with memory maps and close fds

//:~
