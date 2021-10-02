//! csv field parsers, copyright (c) 2021 regents of kparc, bsd-2-clause
#pragma once

#define KQ -1      //!< custom type (fixlen=15, maps to KJ)

U wr(I d,S b,U n); //!< write(2)

typedef unsigned int(*PRS)(struct pjob*);         //!< common interface for field parsers
typedef struct pjob{J jid;G lbl;PRS p;S s;IT*i;U rct;U strd;K x;U skip;I fd;U*tbytes;}pjob;// parser job: jobid,typelabel,parserfn,input,indices,rowcount,stride,dest,offset,bytesparsed
//typedef struct csverr{U filepos;U row;I fld;H fldpos;H errno;}csverr; //!< TODO

U ymdt(S s,UI t,UI*n,U*r,const __m128i*asc,const __m128i*mul10); //!< date|datetime

#ifdef PRS_IMPL

//! low-level parse routines
Zin ZJ _uj(UI n,S s){G c;J r=0;W((c=*s++)&&dgt(c))r=r*10u+((UI)c-'0');R r;}Zin ZJ uj(UI n,S s){P('-'==*s,-(J)_uj(n-1,s+1))R _uj(n,s);}  //<! classic scalar int parser
#define PU(f,t) Zin S ui(S s,J i){J j;do*--s='0'+i-10*(j=i/10);W(i=j);R s;} //!< parse signed/unsigned long long

#include"fdp.h"
//! building blocks for column parsers
#define REA() S base=xG-8;UI nbsz=xn=xn+jb->rct;H nt=NW[xt];Z(0>nt,nbsz/=(-nt);nbsz++;nt=1);base=realloc(base,8+nbsz*nt);zU[jb->jid]=x=8+base; //!< grow result buffer
#define LP(a...) pjob JB=*jb;U t,total=0;S s=JB.s;Nj(JB.rct,t=j*JB.strd;IT idx=JB.i[t],fl=JB.i[t+1]-idx-1;total+=fl;U pos=j+JB.skip;({a;}))\
 Z(3<JB.fd,U bsz=BSZ(xt,JB.rct),skp=BSZ(xt,JB.skip);wr(JB.fd,xG+skp,bsz))A(add)(&jb->tbytes,total,0);           //<! parser loop
#define PRS(T,a...) ZI u##T(pjob*jb){K z=jb->x,x=zU[jb->jid];REA();a;R 0;}              //!< common parser template: grow result buffer, enter main loop
#define PRSN(t) PRS(t,LP(*(x##t+pos)=!fl?0:(t)uj(fl,s+idx)))                            //!< template for integer parsers, int8|32|64 all use uj()
#define DTRIG() const __m128i asc=_mm_set1_epi8('0'),mul10=_mm_setr_epi8(100,1,10,1,0,10,1,0,10,1,0,10,1,0,10,1); //<! bootstrap for vectorized datetime parser
#define PRSDT(t,mode,T) PRS(t,DTRIG()LP(UI l;ymdt(s+idx,mode,&l,x##T+pos,&asc,&mul10))) //!< template for date|datetime parsers

//! field parsers
PRS(S,LP(*(xU+pos)=us(fl,s+idx)))                                    //!< syms aka names
PRSN(G)PRSN(I)PRSN(J)                                                //!< ints 8|32|64
PRSDT(D,0,I)PRSDT(T,1,U)                                             //!< date|datetime
PRS(F,LP(parse_float(s+idx,(F*)xU+pos)))                             //!< ieee doubles
PRS(B,G ctr=0;LP(xG[pos/8+1]|=(s[idx]=='1')<<ctr;Z(7==ctr++,ctr=0))) //!< packed bools
PRS(Q,ZG sep=0;I r[4];X m0=_mm_set1_epi8('0'),                       //!< fast uint64 fixlen=15 (Q)
 m10={fill(10),10,1,10,1,10,1,10,sep},m100=_mm_setr_epi16(fill(100)),m10000=_mm_setr_epi16(fill(10000));
 LP(Z(!fl,*(xU+pos)=0){X input=(X)_mm_loadu_si128(s+idx)-m0,t1=_mm_maddubs_epi16(input,m10),t2=_mm_madd_epi16(t1,m100),t3=_mm_packus_epi32(t2,t2),t4=_mm_madd_epi16(t3,m10000);
  _mm_storeu_si128(r,t4);U res=((U)(r[0])*100000000u+(U)(r[1]))/10;*(xU+pos)=res;}))
#endif

//#define BIT(i) t|=x[i]<<i                      //!< todo could be more efficient than naive bool packer
//Zin _ pkb(UI*x,S r){UI*y=(UI*)r,t=x[0];N(32,BIT(i))y[0]=t;}   //!<*in+=32;*pw+=4; 32 ints -> to 4 bytes

//:~
