#ifndef _ATOMIC_H
#define _ATOMIC_H

struct quene{
    struct task* task_p;
    struct quene*next;
};

typedef unsigned char atomic;

void atomic_inc(atomic *);
void atomic_dec(atomic *);

unsigned char atomic_test_inc(atomic *);

void spin_lock(atomic *);
void spin_unlock(atomic *);

void task_sleep(struct quene **);
void task_wake(struct quene* *q);




#endif