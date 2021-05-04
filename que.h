//! simple lock-free queue, copyright (c) 2021 regents of kparc, bsd-2-clause
#pragma once

#include"k.h"

typedef struct qnode{_*x;struct qnode*next;}*Qn;
typedef struct qroot{Qn head;Qn tail;volatile UI n;}*Qr;

#define YIELD  100
#define yld() _yld(YIELD)

Qr que();UI enq(Qr q,_*x);_*deq(Qr q);_ _yld(UI usec); //!< new, enqueue, dequeue, yield

#ifdef QUE_INTERNAL
#define U(b)        Z(b,break)
#endif

//http://people.cs.pitt.edu/~jacklange/teaching/cs2510-f12/papers/implementing_lock_free.pdf

//:~
