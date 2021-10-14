/*  "the world's fastest hash table"

    first, we use pos=hash1(key) to approximate the bucket location.
    second, we use sig=hash2(key) to perform a linear scan.

    if there is a bucket found, we return it.
    if we encounter a bucket with sig=0,
    we return it as a new position for insert.

    below, we use hash1=wyhash64() and hash2=wy2u0k(wyhash64())
    combined, they provide 128-bit collision resistance.

    some gotchas:

    (!) table does not store keys, only hashes and values, and must be pre-zeroed.
    (!) operations are not thread-safe, lockless version is trickier.
    (!) collisions are unhandled, however unlikely.
    (!) dynamic resize is not implemented.

    (*) hash1 and hash2 can be the same function, which will impact resistance.
    (*) hash values are 64 bit, but can be dropped to 32 if necessary to save space.
    (*) the above two can be enabled with WYHASHMAP_WEAK_SMALL_FAST in the original implementation

    author: 王一 Wang Yi <godspeed_china@yeah.net>
    contributors: Reini Urban, Dietrich Epp, Joshua Haberman, Tommy Ettinger, Daniel Lemire, Otmar Ertl, cocowalla, leo-yuriev, Diego Barrios Romero, paulie-g, dumblob, Yann Collet, ivte-ms, hyb, James Z.M. Gao, easyaspi314 (Devin), TheOneric
    free and unencumbered software released into the public domain.

    #include<stdlib.h>  //calloc
    #include<stdio.h>   //printf
    #include"wyh.h"

    int main(){
     U size=213432;                                     //!< dynamic growth is not supported
     U*idx=calloc(sizeof(U),size);                      //!< allocate a table of fixed size, which must be zeroed.
     U*val=calloc(sizeof(U),size);                      //!< we only care about index, values are managed by user code.
     char*key="dhskfhdsj";                              //!< some key to be inserted
     U sec=0xcafe;                                      //!< some secret
     U pos=wymap(idx,size,key,sl(key),SET,&sec);        //!< insert key and get its position
     Z(pos<size,val[pos]++)QQ("full");                  //!< check capacity, process the value
     O("(%lld) %s -> %lld\n",pos, key,val[pos]);
     pos=wymap(idx,size,key,sl(key),GET,&sec);          //!< lookup position by key
     O("(%lld) %s -> %lld\n",pos, key,val[pos]);
     free(idx),free(val);}

    //! gcc -O3 -g -owy wy.c && ./wy
*/

#include"string.h" //!< memcpy

#if LLL_STANDALONE

#define Zin __attribute__((always_inline)) inline
#define W(a...) while(a)
#define N(n,a...) {U _=(n),i=0;W(i<_){a;++i;}}
#define P(b,a...) if(b)R({a;});
#define Z(b,a...) if(b){a;}else
#define QQ(e) R O("!%s\n",e),1;
#define R return
#define O printf
#define sl strlen

#define SET 1
#define GET 0

typedef unsigned long long U;typedef void _;typedef unsigned char G;typedef char*S;typedef unsigned UI;
#else
#include"k.h"
#undef xn
#endif


#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
  #define likely(x)  __builtin_expect(x,1)
  #define unlikely(x)  __builtin_expect(x,0)
#else
  #define likely(x) (x)
  #define unlikely(x) (x)
#endif

Zin U wyr8(S p){U v;R memcpy(&v,p,8),v;}Zin U wyr4(S p){UI v;R memcpy(&v,p,4),v;}
Zin U wyr3(S p,U k){R(((U)p[0])<<16)|(((U)p[k>>1])<<8)|p[k-1];}Zin U wyrot(U x){R(x>>32)|(x<<32);}
Zin _ wymum(U*A,U*B){__uint128_t r=*A;r*=*B;*A=(U)r,*B=(U)(r>>64);} //!< 128-bit mult
Zin U wymix(U A,U B){R wymum(&A,&B),A^B;}Zin U wy2u0k(U r,U k){wymum(&r,&k);R k;} //!< fast int prng on [0,k) due to lemire. may not work when WYHASH_32BIT_MUM=1. can be combined with wyrand, wyhash64 or wyhash.

Zin U wyhash(_*x,U xn,U seed,U*sec){S p=(S)x;seed^=*sec;U a,b;
 Z(likely(xn<=16),
  Z(likely(xn>=4),a=(wyr4(p)<<32)|wyr4(p+((xn>>3)<<2));b=(wyr4(p+xn-4)<<32)|wyr4(p+xn-4-((xn>>3)<<2)))
   Z(likely(xn>0),a=wyr3(p,xn),b=0)a=b=0;
 ){
  U i=xn; 
  if(unlikely(i>48)){
    U see1=seed,see2=seed;
    do{
     seed=wymix(wyr8(p+ 0)^sec[1],wyr8(p+ 8)^seed);
     see1=wymix(wyr8(p+16)^sec[2],wyr8(p+24)^see1);
     see2=wymix(wyr8(p+32)^sec[3],wyr8(p+40)^see2);
     p+=4;i-=48;
    }W(likely(i>48));
    seed^=see1^see2;
  }
  W(unlikely(i>16)){seed=wymix(wyr8(p)^sec[1],wyr8(p+8)^seed),i-=16,p+=16;}
  a=wyr8(p+i-16),b=wyr8(p+i-8);
 }
 R wymix(sec[1]^xn,wymix(a^sec[1],b^seed));}

//! prng that passes BigCrush and PractRand
Zin U wyrand(U*seed){R*seed+=0xa0761d6478bd642full,wymix(*seed,*seed^0xe7037ed1a0b428dbull);}

U wymap(U*idx,U idxn,S x,U xn,G ins,U*sec){U i=1,h2,sig;
 do{sig=h2=wyhash(x,xn,i++,sec);}W(unlikely(!sig));
 U i0=wy2u0k(wyhash(x,xn,0,sec),idxn);
 for(i=i0;i<idxn&&idx[i]&&idx[i]!=sig;i++);
 Z(unlikely(i==idxn),
  for(i=0;i<i0&&idx[i]&&idx[i]!=sig;i++);
  P(i==i0,idxn));
 Z(!idx[i],
  P(!ins,idxn)
  idx[i]=sig
 );
 R i;}

//:~
