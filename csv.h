#define PRS_IMPL

#include"___.h"
#include"prs.h"
#include"thr.h"
#include"clk.h"
#include"que.h"

typedef struct CSV{I d;UI batch;G sep;G quo;S b;U bn;IT*i;U n;I cct;I rea;U more;F bpf;F rtime;U rbytes;}CSV;

U tok(CSV*r);

#define crlf(c) (c=='\r'||c=='\n')              //!< newline
#define QQ(c,s) Z(c,O("!%s\n",s),exit(1));      //!< fatal

#define DBG
#ifdef DBG
_ trc(CSV*c){N(c->n,O("[%.*s]",c->i[i+1]-c->i[i]-1,c->b+c->i[i]);O("%c",((i+1)%c->cct)?c->sep:'\n'))O("\n");}//!< tokenizer debug
K dbg(S s,K r){O("%s t=%-2d n=%d -> %llx \n",s,rt,rn,rU[-1]);N(rn,K y=r1(rU[i]);O("  %s t=%-2d n=%d -> ",s,yt,yn);O("%llx\n",yU[-1]));R r;}//!< result object
#define bits(t,one)Zin _ OB##t(t n){N(sizeof(t)*8,O("%c%c",(n&(one<<i))?'1':'0',(((i+1)%32))?' ':'\n'))}
bits(U,1LL)bits(G,1)
#endif

//:~
