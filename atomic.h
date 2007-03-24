#ifndef _ATOMIC_H
#define _ATOMIC_H

typedef unsigned char atomic;

void atomic_inc(atomic *);
void atomic_dec(atomic *);

unsigned char atomic_test_inc(atomic *);

void spin_lock(atomic *);
void spin_unlock(atomic *);

void task_sleep(unsigned char *);
void task_wake(unsigned char * q);




#endif