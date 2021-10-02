#include"csv.h"

#define TRC   0
#define RDS   1000

#define BATCH 1e7 //!< rows per read+tokenize iteration (stage 1)

static Qr Q;ZJ N; //!< job (q)ueue, (n)umber of running threads

_ thr(U id){pjob*jb;W(1){jb=(pjob*)deq(Q);Z(jb,Z(jb->jid<0,break);jb->p(jb);A(sub)(&N,1,0);continue)yld();}}    //!< parser thread
_ cpu(I n){Z(Q,R)Z(0<n,Q=que();N(n,launch(thr,i))){pjob jb={-1};N(-n,enq(Q,&jb))join_all();free(Q);}}           //!< launch/join parser threads

#define MAP(c,t,f) C(c,typ[i]=t==KC?0:t==KQ?KJ:t;y=rU[cct++]=tn(typ[i],1);yn=1;Z(t==KB,yG[-8]=1);prs[i]=(PRS)u##f)   //!< map coltype specifier to corresponding parser function

ZK prs(I fd,I ncol,I hct,I ncpu,U skp,G sep,S fmt,I*fds,prof*prf){BENCH();UI jcnt,rct,cct=0,typ[ncol];PRS prs[ncol];K y,r=tn(0,hct);U bytes_parsed=0;

	N(ncol,S(fmt[i],MAP('b',KB,B)MAP('g',KG,G)MAP('i',KI,I)MAP('j',KJ,J)MAP('f',KF,F)MAP('s',KS,S)MAP('d',KD,D)MAP('t',KT+8,T)MAP('Q',KQ,Q)C('*',typ[i]=prs[i]=0),QQ(1,"!type")))

	QQ(cct-hct,"length") //!< disparity between colname count and colformat count is hard error

	//    {I d;UI batch;G sep;G quo;S b;U bn;IT*i;U n;I cct;I rea;U more;F bpf;F rtime;U rbytes;}           //!< tokenizer re-entry struct
	CSV c={ fd,   BATCH,  sep,    0,  0,   0,   0,  0, ncol,    0,     1,  1.0,    0.0,       0};

	pjob jobs[cct];ncpu=ttl(cores)=min(cct,ncpu);

	W(c.more){
		jcnt=c.rtime=0;
		WALL(tok(&c))ttl(rtime)+=c.rtime;ttl(ttime)+=wall-c.rtime;

		Z(TRC,trc(&c));  //!< trace tokenizer output

		rct=c.n/ncol;

		//                              {J jid;G lbl;    PRS p; S s;  IT*i;        U rct;U strd;K x;U skip; I fd;       U*tbytes; }pjob;
		N(ncol,Z(typ[i],jobs[jcnt]=(pjob){jcnt,fmt[ncol],prs[i],c.b,c.i+skp*ncol+i,rct-skp,ncol,r,ttl(rows),fds[jcnt++],&bytes_parsed}))

		cpu(ncpu);                                  //!< spinup ncpu idle parser threads, if not yet
		N(cct,pjob*jb=jobs+i;jb->s=c.b;enq(Q,jb))   //!< enqueue parser jobs, aka stage 2
		skp=0;ttl(rows)+=rct;N=cct;                 //!< unset skip after first batch
		WALL(W(N)yld())ttl(ptime)+=wall;            //!< fixme block until all parsers are done

		Z(TRC&&prf->total_rows>=RDS*BATCH,break);   //!< stop after first RDS batches
	}
	ttl(bytes)=c.rbytes;ttl(parsed)=bytes_parsed;   //!< update stats
	R cpu(-ncpu),free(c.i),free(c.b),               //!< terminate parser threads, release index and input buffers
		cork(fds,r,ttl(rows));}                     //!< return a vector of mmap'ed splayed columns

//! read a line from fd and parse it as a sym vector of colnames, skipping spaces, slashes and dots
ZK cnm(I fd,I ncol,G sep,S fmt){K z=tn(KS,ncol);UI ln=0,n=1000,e;G c,b[n+1],sp[4]={'\r','\n',sep,0};
	W(n){e=read(fd,b+ln,1);QQ(0>e,"read")c=b[ln++];Z(!e||crlf(c),break){Z(c==' '||c=='\t'||c=='.'||c=='/',--ln);--n;}}
	n=e=0;stok(b,ln,sp,Z(fmt[n]!='*',((U*)z)[e++]=us(tn,t));n++)R zn=e,z;}

EXP K csv(K r){BENCH(); //!< entry point   (argparse + i/o + bench)   r:(infile;formatspec;colnames;outdir;maxcore;skip)
	K x=rU[0],y=rU[1],z=rU[2],f=rU[3];I ncpu=max(1,min((I)rU[4],NCPU)),skp=rU[5],ncol=yn-1,fd=opr(x);G*fmt=yG,sep=*fmt++;

	QQ(!z&&skp,"colnames")Z(!z&&!skp,z=cnm(fd,ncol,sep,fmt);skp=0); //<! if column names are not explicit, parse them from 0th row

	I*fds=opw(f,z); //!< open output descriptors in directory f

	O("fname=%.*s fmt=%.*s dir=%.*s ncol=%d ncpu=%d skp=%d sep=%c fd=%d\n",xn,xG,ncol,fmt,n_(f),(S)f,ncol,ncpu,skp,sep,fd);

	prof*prf=new(prof);WALL(r=prs(fd,ncol,zn,ncpu,skp,sep,fmt,fds,prf))ttl(wtime)=wall;

	O("file : %.*s     ",      xn,xG);
	O("%.2f (%.2f)GB    ",     (F)ttl(parsed)/GiB,(F)ttl(bytes)/GiB);
	O("rows: %llu    ",        ttl(rows));
	O("cores: %d(%d)\n\n",     ttl(cores),NCPU);

	O("wall  : %.2fs\nread  : %.2fs\nscan  : %.2fs\nparse : %.2fs\n\n", ttl(wtime),ttl(rtime),ttl(ttime),ttl(ptime));
	O("r/s/p/total : %0.2f %0.2f %0.2f %0.2f GB/s\n\n",GBs(bytes,rtime),GBs(bytes,ttime),GBs(bytes,ptime),GBs(bytes,wtime));

	R free(prf),close(fd),tab(z,r);}

//! dummy ffi rountrip
K rtrip(K r){dbg("rtrip",r);R r1(r);}

//:~
