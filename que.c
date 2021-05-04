//! simple lock-free queue, copyright (c) 2021 regents of kparc, bsd-2-clause
#define QUE_INTERNAL
#include"que.h"

#define New(t) ({_*r=calloc(1,sizeof(struct t));P(!r,exit(1),NULL);r;})
#define cas __sync_bool_compare_and_swap
Qr que(){Qr root=(Qr)New(qroot);root->tail=root->head=(Qn)New(qnode);R root->head->x=root->head->next=0,root;}//!< sentinel
UI enq(Qr root,_*x){Qn n,node=(Qn)New(qnode);node->x=x;node->next=0;W(1){n=root->tail;U(cas(&n->next,0,node))cas(&root->tail,n,n->next);}R cas(&root->tail,n,node),1;}
_* deq(Qr root){Qn n;_*x;W(1){n=root->head;P(!n->next,0L)U(cas(&root->head,n,n->next));}R x=n->next->x,free(n),x;}

_ _yld(UI usec){
	Z(!usec,
#ifndef __aarch64__
    __builtin_ia32_pause();
#else
    asm volatile("yield");
#endif
	)usleep(usec);
}

//:~
