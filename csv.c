#include"csv.h"

static Qr Q;ZJ N; //!< job (q)ueue, (n)umber of running threads

I opn(K x){S f=calloc(1,xn+1);memcpy(f,xG,xn);I r=access(f,F_OK);Z(!r,goto OP);r=mkfifo(f,0600);QQ(-1==r,"fifo")OP:R r=open(f,O_RDONLY,0),free(f),r;} //!< open file or create fifo
_ thr(U id){pjob*jb;W(1){jb=(pjob*)deq(Q);Z(jb,Z(jb->jid<0,break);jb->p(jb);A(sub_fetch)(&N,1,0);continue;)yld();}} //!< parser thread
_ cpu(I n){Z(Q,R)Z(0<n,Q=que();N(n,launch(thr,i))){pjob jb={-1};N(-n,enq(Q,&jb))join_all();free(Q);}}               //!< launch/join parser threads

#define MAP(c,t,f) C(c,typ[i]=t==KC?0:t==KQ?KJ:t;y=rU[cct++]=tn(typ[i],1);Z(t==KB,yG[-8]=1);prs[i]=(PRS)u##f)    //!< map format specifier to a parser function
ZK prs(I fd,I ncol,K cnames,I ncpu,U skp,G sep,S fmt,prof*prf){BENCH();UI jcnt,hct=n_(cnames),cct=0,typ[ncol];PRS prs[ncol];K y,r=tn(0,hct);
	N(ncol,S(fmt[i],MAP('b',KB,B)MAP('g',KG,G)MAP('i',KI,I)MAP('j',KJ,J)MAP('f',KF,F)MAP('s',KS,S)MAP('d',KD,D)MAP('t',KT+8,T)MAP('Q',KQ,Q)C('*',typ[i]=prs[i]=0),QQ(1,"!type")))

	QQ(cct-hct,"length")

    //    {I d;UI batch;G sep;G quo;S b;U bn;IT*i;U n;I cct;I rea;U more;F bpf;F rtime;U rbytes;}   //!< tokenizer re-entry
	CSV c={ fd,     1e7,  sep,    0,  0,   0,   0,  0, ncol,    0,     1,  1.0,    0.0,       0};

	pjob jobs[cct];ncpu=ttl(cores)=min(cct,ncpu);U bytes_parsed=0;

	W(c.more){
		jcnt=c.rtime=0;
		WALL(tok(&c))ttl(rtime)+=c.rtime;ttl(ttime)+=wall-c.rtime;

		//trc(&c);

		UI rct=c.n/ncol;
		N(ncol,Z(typ[i],jobs[jcnt]=(pjob){jcnt++,fmt[ncol],prs[i],c.b,c.i+skp*ncol+i,rct-skp,ncol,r,ttl(rows),&bytes_parsed}))
		cpu(ncpu);N(cct,pjob*jb=jobs+i;jb->s=c.b;enq(Q,jb))
		skp=0;ttl(rows)+=rct;N=cct;
		WALL(W(N)yld())ttl(ptime)+=wall;

		//Z(prf->total_rows>=1e6,break);
	}
	ttl(bytes)=c.rbytes;ttl(parsed)=bytes_parsed;dbg("out",r);
	R cpu(-ncpu),free(c.i),free(c.b),tab(cnames,r);}

//! read a line from a file descriptor and parse it into into a vector of column names, skipping spaces
ZK cnm(I fd,I ncol,G sep,S fmt){K z=tn(KS,ncol);UI ln=0,n=1000,e;G c,b[n+1],sp[4]={'\r','\n',sep,0};
	W(n){e=read(fd,b+ln,1);QQ(0>e,"read")c=b[ln++];Z(!e||crlf(c),break){Z(c==' ',--ln);--n;}}
	n=e=0;stok(b,ln,sp,Z(fmt[n]!='*',((U*)z)[e++]=us(tn,t););n++)R zn=e,z;}

//! ffi entry point (mostly benchmarking)
EXP K csv(K r){BENCH();
	K x=rU[0],y=rU[1],z=rU[2];I CR=sysconf(_SC_NPROCESSORS_ONLN),ncpu=max(1,min((I)rU[3],CR)),skp=rU[4],fd=opn(x),ncol=yn-1;G*fmt=yG,sep=*fmt++;
	QQ(!z&&skp,"cnames")Z(!z&&!skp,z=cnm(fd,ncol,sep,fmt);skp=1); //<! if column names are not explicit, infer them from the first row

	O("fname=%.*s fmt=%.*s ncol=%d ncpu=%d skp=%d sep=%c fd=%d\n",xn,xG,ncol,fmt,ncol,ncpu,skp,sep,fd);

	prof*prf=new(prof);WALL(r=prs(fd,ncol,z,ncpu,skp,sep,fmt,prf))ttl(wtime)=wall;

	O("file : %.*s     ",      xn,xG);
	O("%.2f (%.2f)GB    ",     (F)ttl(parsed)/GiB,(F)ttl(bytes)/GiB);
	O("rows: %llu    ",        ttl(rows));
	O("cores: %d(%d)\n\n",     ttl(cores),CR);

	O("wall  : %.2fs\nread  : %.2fs\nscan  : %.2fs\nparse : %.2fs\n\n", ttl(wtime),ttl(rtime),ttl(ttime),ttl(ptime));
	O("r/s/p/total : %0.2f %0.2f %0.2f %0.2f GB/s\n\n",GBs(bytes,rtime),GBs(bytes,ttime),GBs(bytes,ptime),GBs(bytes,wtime));

	R free(prf),close(fd),r;}

//! dummy ffi rountrip
K rtrip(K r){dbg("rtrip",r);R r1(r);}

//:~
