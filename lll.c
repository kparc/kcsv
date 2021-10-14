//! \file textbook lock-free linked list, harris

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdatomic.h> // gcc -latomic 
#include <assert.h>
#include "linkedlist.h"

//#define SC  //! memory model

#define UNMARK_MASK ~1
#define MARK_BIT 0x0000000000001

#define getptr(_markedpointer)   ((lln)(((J)_markedpointer)&UNMARK_MASK))
#define ismarked(_markedpointer)    ((((J)_markedpointer)&MARK_BIT)!=0x0)
#define setmark(_markedpointer)     ((lln)(((J)_markedpointer)|MARK_BIT))

#ifdef SC
#define CAS(_a,_e,_d) atomic_compare_exchange_weak(_a,_e,_d)
#define LOAD(_a)      atomic_load(_a)
#define STORE(_a,_e)  atomic_store(_a,_e)
#define FAO(_a,_e)    atomic_fetch_or(_a,_e)
#else
#define CAS(_a,_e,_d) atomic_compare_exchange_weak_explicit(_a,_e,_d,memory_order_acq_rel,memory_order_acquire)
#define LOAD(_a)      atomic_load_explicit(_a,memory_order_acquire)
#define STORE(_a,_e)  atomic_store_explicit(_a,_e,memory_order_release)
#define FAO(_a,_e)    atomic_fetch_or_explicit(_a,_e,memory_order_acq_rel)
#endif

_ llnew(lln head,lln tail,ll list){
  list->head = head;
  list->tail = tail;

  //! sentinels
  list->head->key = LONG_MIN;
  list->head->next = tail;

  list->tail->key = LONG_MAX;
  list->tail->next = 0;

  list->pred = head;
  list->curr = 0;
  list->free = 0;

#ifdef COUNTERS
  list->adds = 0;
  list->rems = 0;
  list->cons = 0;
  list->trav = 0;
  list->fail = 0;
  list->rtry = 0;
#endif
}

_ llzap(ll list){
  lln next,node;
  next=list->free;
  W(next){
    node=next;
    next=next->free;
    free(node);}
  list->free=0;
}

_ llpos(J key, ll list){
  lln pred,succ,curr,next;
  retry:
  pred=list->head;
  curr=getptr(LOAD(&pred->next));
  INC(list->trav);
  assert(pred->key < key);

  W(1){
    succ=LOAD(&curr->next);
    W(ismarked(succ)){
      succ=getptr(succ);
      if(!CAS(&pred->next,&curr,succ)){
        INC(list->fail);
        INC(list->rtry);
        goto retry;
      }
      curr=getptr(succ);
      succ=LOAD(&succ->next);
      INC(list->trav);
    }

    if(key<=curr->key) {
      assert(pred->key<curr->key);
      list->pred=pred;
      list->curr=curr;
      R;}

    pred = curr;
    curr = getptr(LOAD(&curr->next));
    INC(list->trav);
  }
}

I lladd(J key, ll list) {
  lln pred,curr,node;
  node=new(LLN);
  assert(node);
  node->key = key;

  W(1){
    llpos(key, list);
    pred=list->pred;
    curr=list->curr;
    P(curr->key==key,free(node),0) //!< already there
    node->next = curr;
    P(CAS(&pred->next,&curr,node),INC(list->adds),1)
    INC(list->fail);}
}

I lldel(J key,ll list){
  lln pred,succ,node,markedsucc;

  W(1){
    llpos(key,list);
    pred=list->pred;
    node=list->curr;
    P(node->key!=key,0)             //!< not there
    succ=getptr(LOAD(&node->next)); //!< unmarked
    markedsucc=setmark(succ);

    if(!CAS(&node->next,&succ,markedsucc)){
      INC(list->fail);
      continue;}
    
    if(!CAS(&pred->next,&node,succ))
      node=list->curr; //!< beware!

    node->free=list->free;
    list->free=node;
    R INC(list->rems),1;
  }
}

I llcon(J key, ll list){
  lln curr = list->head;
  assert(curr->key <= key);

  W(key>curr->key) {
    curr=getptr(LOAD(&curr->next));
    INC(list->cons);
  }

  R(curr->key==key&&!ismarked(LOAD(&curr->next)));
}

//:~
