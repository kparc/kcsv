// a (very) quick and dirty standalone smoke test
#include"k.h"
#define KC(x,s) {UI sn=strlen(s);K r=8+calloc(1,8+sn);rn=sn,rt=KC,memcpy(rG,s,sn);x=r;}

K csv(K x);Zin U us(UI n,S s){R!n?0:*(U*)s&~0ULL>>64-8*min(8,n);}
S file="EQY_US_ALL_TRADE_20201210",frmt="|QsssifsgijssQQb";
const I CCT=15;S cnames[CCT]={"tme","xch","sym","cnd","vol","trd","stp","crr","seq","tid","src","rpt","ptm","rtm","xpt"};

int main(){K x,y,z;
 KC(x,file);KC(y,frmt);z=8+calloc(1,8+sizeof(U)*CCT);zn=CCT,zt=KS;N(CCT,S s=cnames[i];zU[i]=us(strlen(s),s))
 K r;{K x=8+calloc(1,8+8*5);xn=5;xt=0;r=x;}rU[0]=x,rU[1]=y,rU[2]=z,rU[3]=sysconf(_SC_NPROCESSORS_ONLN)/2,rU[4]=0; //fname,fieldspec,colnames,mxcore,skip
 csv(r);}

//:~
