//! ld() fast csv tokenizer, copyright (c) 2021 regents of kparc, bsd-2-clause
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include"tok.h"
#include"clk.h"

#define BN 8 //buffering rounds for better pipelining

Zin U _readn(I d,S b,U n){U r=0;J e;W(n){e=read(d,b,n);P(e<0,O("!read =%d b=%p u=%d %d %s\n",d,b,n,e,strerror(errno)),exit(1),0)P(!e,r)b+=e,n-=(U)e,r+=(U)e;};R r;}
Zin U readn(I d,S b,U n,CSV*c){BENCH();U r=0;WALL(r=_readn(d,b,n))R c->rbytes+=r,c->rtime+=wall,r;}

#define SHR(m) m^=*in_q,*in_q=((J)(m))>>63,m //assumes shr(J) well-defined
#ifdef __ARM_NEON
Zin U cmi(span z,G m){vec vm=vset1(m);R mmask(cmpeq(z.i0,vm),cmpeq(z.i1,vm),cmpeq(z.i2,vm),cmpeq(z.i3,vm));}
Zin U fqm(span x,U*in_q){U qbts=cmi(x,'"');U m=vmull(-1ULL,qbts);R SHR(m);} //no vmull on rpi4
#else
Zin U cmi(span z,G m){vec b=vset1(m);U x=(UI)mmask(cmpeq(b,z.l)),y=mmask(cmpeq(b,z.h));R(y<<32)|x;}
Zin U fqm(span x,U*in_q){U qbts=cmi(x,'"');U m=vmull(aV(qbts,0),aV(-1ULL),0)[0];R SHR(m);}
#endif

#define z(i) p[ba+i]=1+idx+_(ctzll)(b),b&=b-1;//O("n %2d  i %2d  ba+i %-6d  p[ba+i] %-6d  %d\n",n,i,ba+i,p[ba+i],p[ba+i]-idx-1);
Zin IT zip(IT*p,IT ba,IT idx,U b){P(!b,ba)IT n=ham(b);N(8,z(i))Z(n>8,N(8,z(i+8))Z(unlikely(n>16),ba+=16;do{z(0)ba++;}W(b)));R ba+n;}

#define mask(dest) qt_mask=r->quo?fqm(in,&in_qt):0,sep=cmi(in,r->sep),trm=cmi(in,'\n');dest=(trm|sep)&~qt_mask;  //!< nocrlf
//#define mask(dest) qt_mask=r->quo?fqm(in,&in_qt):0,sep=cmi(in,r->sep);U cr=cmi(in,0x0d),cr_adj=(cr<<1)|prev_iter_cr_end,lf=cmi(in,0x0a);trm=lf&cr_adj;prev_iter_cr_end=cr>>63;dest=(trm|sep)&~qt_mask;

#ifndef NOAVX

/*
_ seed(CSV*r){
 U watermark=r->batch*r->cct,ADJ=watermark/r->bpf;
 r->b=(S)malloc(r->bn=ADJ),
 r->i=(IT*)malloc(r->in=sizeof(IT)*(watermark+1024*2*1)),
 *r->i=0,r->i++,
 r->bn=r->more=readn(r->d,r->b,r->bn,r);
 printf("seed alloc r->in %llu r->bn %llu r->i %p\n",r->in,r->bn,r->i);}
*/

Zin _ grow(CSV*r,U base,U wm){
	if(!base){base=1;}
	r->bpf=(F)r->bn/base;
	//printf("underflow bn %llu    base = %llu   water = %llu  BPF = %f   fix=%llu\n",r->bn, base, wm, r->bpf, (U)ceil((wm-base)*r->bpf));
	U fix=(U)ceil((wm-base)*r->bpf);
	r->b=realloc(r->b,r->bn+fix+256);
	memset(r->b+r->bn,0,fix+256);
	//printf("r->b+r->bn %llu\n",fix);
	r->more=readn(r->d,r->b+r->bn,fix,r);
	r->bn+=r->more;r->rea++;}

//r->bn=r->bn<64?64:r->bn;
U tok(CSV*r){span in;U intl_idx,sep,trm,qt_mask,f_sep,idx=0,base=0,in_qt=0,prev_iter_cr_end=0;IT*base_ptr=r->i;
 U watermark=r->batch*r->cct;
 if(!r->b){
	U ADJ=watermark/r->bpf;
	r->b=(S)malloc(r->bn=ADJ+128),
	r->i=base_ptr=(IT*)malloc(sizeof(IT)*(watermark+1)),
	*base_ptr=0,base_ptr++,r->bn=r->more=readn(r->d,r->b,r->bn,r);
	printf("seed alloc r->bn %llu r->i %p\n",r->bn,r->i);
 } else {
	IT last=r->i[watermark-1];U taillen=r->bn-last;
	//printf("bn %llu r->n %llu i[n+1] %d taillen %llu more %llu\n", r->bn, r->n, last, taillen,r->more);
	memmove(r->b,r->b+last,taillen);r->bn=taillen+(r->more=readn(r->d,r->b+taillen,r->bn-taillen,r));
 }

 if(BN&&r->bn>64*BN){U fields[BN];for(;idx<r->bn-64*BN+1;idx+=64*BN){ //eliminated if BN is zero
	N(BN,intl_idx=64*i+idx;_(prefetch)(r->b+intl_idx+128);in=vld(r->b+intl_idx);mask(fields[i]))
	N(BN,intl_idx=64*i+idx;base=zip(base_ptr,base,intl_idx,fields[i]);)
	Z(base>=watermark,base=watermark;break);}}

 for(;;idx+=64){
	Z(r->bn<idx+64,grow(r,base,watermark));
	_(prefetch)(r->b+idx+128);
	in=vld(r->b+idx);mask(f_sep)base=zip(base_ptr,base,idx,f_sep);
	Z(base>=watermark,base=watermark;break);
	Z(!r->more,break);}

 R r->n=base;}

#else
U csv(CSV*r){r->n=0;G q=0;VN(n,S(s[i],C('"',Z(q&&s[i+1]=='"',i++)q=!q)case',':C('\n',Z(!q,r->i[r->n++]=1+i))))R r->n;}
#endif

//:~
