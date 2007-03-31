#ifndef _ATOMIC_H
#define _ATOMIC_H

struct quene{
    struct task xdata* task_p;
    struct quene xdata*next;
};

typedef unsigned char atomic;

void atomic_inc(atomic pdata*);
void atomic_dec(atomic pdata*);

unsigned char atomic_test_inc(atomic pdata*);

void spin_lock(atomic pdata*);
void spin_unlock(atomic pdata*);

void task_sleep(struct quene xdata*pdata*);
void task_wake(struct quene xdata*pdata*);

void spin_irqsave_lock(atomic pdata*l);
void spin_irqsave_unlock(atomic pdata*l);


#endif