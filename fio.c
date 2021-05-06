//! splayed table i/o, copyright (c) 2021 regents of kparc, bsd-2-clause
#include"fio.h"

#define ST(x) struct stat b

I mkd(S s){I r=mkdir(s,S_IRWXU);QQ(r,s)R r;}
I acc(S s,I f){R access(s,F_OK)+access(s,f?W_OK:R_OK);}
I rwd(I d,U o){R lseek(d,o,SEEK_SET),d;}
U fsz(I d){ST(b);I r=fstat(d,&b);QQ(r,"fsize")R b.st_size;}
U wr(I d,S b,U n){U w=write(d,b,n);QQ(w-n,"write")R w;}

_ ffo(S f){ST(b);I r=stat(f,&b);Z(S_ISFIFO(b.st_mode),O("fifo:%s\n",f));}
I dir(K x){S s=cS(x);ST(b);I r=stat(s,&b);Z(r,mkd(s))QQ(!S_ISDIR(b.st_mode),s)R chdir(s),free(s),0;}
K map(S f,U*n,I d){d=d?rwd(d,0):open(f,O_RDONLY,0);S s=mmap(0,*n=fsz(d),PROT_READ|PROT_WRITE,MAP_PRIVATE,d,0);R close(d),(K)(16+s);}

I opr(K x){S f=cS(x);I r=acc(f,0);Z(!r,goto OP);r=mkfifo(f,0600);QQ(-1==r,"fifo")OP:ffo(f);R r=open(f,O_RDONLY,0),free(f),r;}
I*opw(K x,K y){dir(x);K z,r=tn(KI,yn);S hdr=mem(16);N(yn,z=str(Yu);Ri=open(zG,O_RDWR|O_CREAT|O_TRUNC,0600);QQ(3>Ri,zG)wr(Ri,hdr,16);r0(z);)R free(hdr),r;}

K cork(I*fds,K r,U rct){I d;U y,x[2];N(rn,d=rwd(fds[i],0);y=Ru;xx=fsz(d);xy=yU[-1];wr(d,xG,16),Ru=y=map(0,x,d);)R r;}

//:~
