//! csv date|datetime field parser, copyright (c) 2021 regents of kparc, bsd-2-clause

#include"___.h"
#include"prs.h"

//! subset of rfc3339:
//  a) yyyymmdd[HHMMSS[.uuuuuu]] (scalar version unly)
//  b) yyyy-mm-dd(T|t| )[HH:MM:SS[.uuuuuu]]
//  a|b is by presence of separator at 4th position.

#define SCALAR 0

static enum{e_ok,e_y,e_m,e_d,e_H,e_M,e_S,e_us,e_sep,e_leap,e_tz}dt_err;

#define X(e)      *n=c-s-1,e_##e
#define ip(f,n)   {UI t=0;N(n,Z(dgt(*c),++t,f=10*f+*c++-'0'){c++;R X(f);})P(t>n,c-=t-n,X(f))}
#define usec()    {UI t=0;N(6,Z(dgt(*c),++t,us=10*us+*c++-'0')Z(!t,R X(us))break)W(dgt(*c++));}
#define sp(s)     Z(s,P(!ext,X(sep))c++)P(ext,c++,X(sep))
#define i2(f)     ip(f,2)
#define is(a)     (*c==a)
#define DS        is('-')
#define TS        is(':')
#define US        is('.')
#define DTS       (is('T')||is(' ')||is('t'))
#define TZS       (is('Z')||is('-')||is('+')||is('z'))
#define mb(m)     ((m*306+5)*13107>>17) //courtesy aab

Zin UI ymd(UI y,UI m,UI d){R y-=2001,m=12*y+m+9,m/12*1461/4+mb(m%12)-307+d;}
Zin U ymdt(S s,UI t,UI*n,U*r,const __m128i*asc,const __m128i*mul10){*r=0;S c=s;UI i,l=0,y=0,m=0,d=0,H=0,M=0,S=0,us=0,ext=0,zdt=0,T=0;U Texp=1;
   Z(SCALAR,ip(y,4)P(1>y,X(y))Z(DS,ext=1);sp(DS)i2(m)P(!in(1,m,12),X(m))sp(DS)i2(d)){ //!< yyyy[-]mm[-]dd    scalar
    static uint16_t o[8];const __m128i t0=_mm_subs_epu8(*(__m128i*)s,*asc),t1=_mm_maddubs_epi16(t0,*mul10);_mm_store_si128((__m128i*)o,t1);
    y=10*o[0]+o[1],m=o[2]+o[3],d=o[4],H=o[5]+o[6],M=o[7];P(!in(1,m,12),X(m))c+=17;}   //!< yyyy*mm*dd*HH*MM  vector
   S(m,C(2,Z(28<d,l=!(y%4)&&(y%100||!(y%400));P(!l,X(leap))P(29<d,X(d))))case 4:case 6:case 9:C(11,P(30<d,X(d))),P(31<d,X(d)))Z(!t,goto done)
   Z(SCALAR,sp(DTS)i2(H)sp(TS)i2(M));sp(TS)i2(S)Z(US,c++;usec())                      //!< HH[:]MM[:]SS[.uuuuuu]
   Z(H==24&&!M&&!S&&!US,H=0,zdt=1);P(23<H,X(H))P(59<M,X(M))P(59<S,X(S))P(TZS,X(tz))   //!< time is midnight (tz nyi)
   T=us+1000*(S+60*(M+60*H)),Texp=86.4e6;done:R*r=ymd(y,m,d)*Texp+T,*n=c-s,e_ok;}

//:~
