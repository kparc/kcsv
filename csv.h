#define PRS_IMPL

#include"k.h"
#include"prs.h"
#include"thr.h"
#include"clk.h"
#include"que.h"
#include"fio.h"

typedef struct CSV{I d;UI batch;G sep;G quo;S b;U bn;IT*i;U n;I cct;I rea;U more;F bpf;F rtime;U rbytes;}CSV;

U tok(CSV*r);

#define crlf(c) (c=='\r'||c=='\n') //!< newline

#define DBG
#ifdef DBG
_ trc(CSV*c){I rct=0;N(c->n,Z(!(i%c->cct),O("%6d ",rct++));I fl=c->i[i+1]-c->i[i]-1;O("[%.*s]",fl,c->b+c->i[i]);O("%c",((i+1)%c->cct)?c->sep:'\n'))O("\n");}  //!< tokenizer dbg
K dbg(S s,K r){O("%s t=%-2d n=%d -> %llx \n",s,rt,rn,rU[-1]);N(rn,K y=rU[i];O("  %s t=%-2d n=%d -> ",s,yt,yn);O("%llx\n",yU[-1]));R r;}                 //!< result object
#endif

//:~
