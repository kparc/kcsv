//! \file textbook lock-free linked list, harris

#include"../k.h"

#define COUNTERS     //!< perfctrs

#ifdef COUNTERS
#define INC(_c) ((_c)++)
#else
#define INC(_c)
#endif

typedef struct LLN {
  _Atomic(struct LLN*)next;
  _Atomic(struct LLN*)prev;
  struct LLN*free;    //!< for freelist
  char padding[40];   //!< fill the cacheline
  J key;} LLN;

typedef LLN*lln;

typedef struct LL {
  lln head,tail;      //!< sentinels, possibly shared
  lln curr;           //!< private cursor (last op)
  lln pred;           //!< predecessor of cursor
  lln free;           //!< private free list
#ifdef COUNTERS
  U adds, rems, cons, trav, fail, rtry;
#endif
} LL;

typedef LL*ll;
  
_ llnew(lln head,lln tail,ll list),
  llzap(ll list);
  
I lladd(J key,ll list),
  lldel(J key,ll list),
  llcon(J key,ll list);

//:~
