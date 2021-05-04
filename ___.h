//! public domain
#pragma once

#include<stdlib.h>   // malloc free
#include<unistd.h>   // sysconf usleep
#include<string.h>   // memcpy
#include<stdio.h>    // printf
#include<sys/stat.h> // mkfifo
#include<fcntl.h>    // open
#include<math.h>     // isfinite

#define VEC __attribute__((vector_size(16),aligned(1)))
#define Zin __attribute__((always_inline)) inline
#define Nin __attribute__((noinline))
#define EXP __attribute__((visibility("default")))

#define T(d,t...) typedef d t;
T(void,_)T(long long,J)T(unsigned long long,U,K)T(char,C)T(unsigned char,G,*S,X VEC)T(unsigned int,UI)T(double,F)T(short,H)T(int,I)

//! less is more
#define R           return
#define W(a...)     while(a)
#define P(b,a...)   if(b)R({a;});
#define Z(b,a...)   if(b){a;}else
#define N(n,a...)   {U _n=(n),i=0;W(i<_n){a;++i;}}
#define Nj(n,a...)  {U _n=(n),j=0;W(j<_n){a;++j;}}
#define C(i,a...)   case i:{a;}break;
#define S(i,c,a...) switch(i){c default:{a;}}
#define min(a,b)    ({typeof(b)_a=(a),_b=(b);_b<_a?_b:_a;})
#define max(a,b)    ({typeof(b)_a=(a),_b=(b);_b>_a?_b:_a;})
#define in(l,x,r)   ((l)<=(x)&&(x)<=(r))
#define dgt(c)      in('0',c,'9')
#define O           printf

//! list length
#define n_(x) ((UI*)x)[-1]

//! accessors
#define xt  xG[-5]
#define xn  n_(x)

#define yt  yG[-5]
#define yn  n_(y)

#define zt  zG[-5]
#define zn  n_(z)

#define rt  rG[-5]
#define rn  n_(r)

#define xI  ((UI*)x)
#define xG  ((S)x)
#define Xg  xG[i]

#define yI  ((UI*)y)
#define yG  ((S)y)
#define Yg  yG[i]

#define zG  ((S)z)
#define Zg  zG[i]

#define rG  ((S)r)

#define xx  xU[0]
#define xy  xU[1]
#define xz  xU[2]

#define xJ  ((J*)x)
#define xU  ((K*)x)
#define yU  ((K*)y)
#define zU  ((K*)z)
#define rU  ((K*)r)

//! statics
#define ZK static K
#define ZJ static J
#define ZH static H
#define ZC static C
#define ZI static I
#define ZU static U
#define ZG static G
#define ZF static F

//! atomics
#define _(x) __builtin_##x
#define A(x) __atomic_##x

//! avx==arm
#include"vec.h"

//! type system
enum{KC=1,KS=2,KB=8,KG=11,KI=13,KJ=14,KD=25,KT=29,KF=43,XT=253};
ZC NW[48]={8,8,8,0,0,0,0,0,-8,0,0,1,0,4,8,0,0,0,0,0,0,0,0,0,0,4,0,0,0,8,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0}; //!< native type widths
//         0 1 2 3 4 5 6 7  8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
//         K C S            B     G   I J                     D       T                           F

//! easy strtok: (s)tr le(n) (d)elims e(x)ec: exec is arbitrary code, has access to t,tn,tp: token, token length, token position
#define stok(s,n,d,x){ZG _z=0,D[255]="*";Z(!_z,_z=1;N(strlen(d),D[d[i]]='*'));{\
 UI tn,_in=0;U tp=0;S t;N(n+1,Z(D[s[i]],Z(_in,_in=0,t=s+tp,tn=i-tp;{x;})tp=i)Z(!_in,_in=1,tp=i))}}

Zin ZK tn(I t,I n){K x;H nt=NW[t];U bn=n;Z(0>nt,bn/=-nt;nt=1);R x=(K)(8+(S)calloc(1,8+nt*bn)),xt=t,xn=n,x;} //!< allocate faux buddy block
Zin K r1(K x){R A(fetch_add)(&xG[-6],1,0),x;}                    //!< increment refcount
Zin K tab(K keys,K cols){K x=tn(0,2);R xt=XT,xx=keys,xy=cols,x;}   //!< construct a table

#define new(t) malloc(sizeof(t))

//typedef U IT;
typedef UI IT;

//:~
