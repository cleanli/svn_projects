#ifndef _ATOMIC_H
#define _ATOMIC_H

struct quene{
    struct task xdata* task_p;
    struct quene xdata*next;
};

typedef unsigned char atomic;

void atomic_inc(atomic xdata*);
void atomic_dec(atomic xdata*);

unsigned char atomic_test_inc(atomic xdata*);

void spin_lock(atomic xdata*);
void spin_unlock(atomic xdata*);

void task_sleep(struct quene xdata**);
void task_wake(struct quene xdata**);




#endif