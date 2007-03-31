#include "atomic.h"
#include "at89x51.h"
#include "gernel.h"
#include "mm.h"
#include <stdlib.h>

xdata atomic task_data_lock;

static xdata unsigned char ea_bak;

void int_disable_store()
{
    ea_bak=EA?1:0;
    EA=0;
}

void int_restore()
{
    EA=(ea_bak!=0)?1:0;
}

void switch_task()
{
    soft_int=1;
    TF0=1;
}
/*
void atomic_inc(atomic * s)
{
    CY=EA;
	EA=0;
	s++;
	EA=CY;
} */

void atomic_dec(atomic xdata* s)
{
    int_disable_store();
	(*s)--;
	int_restore();
}

unsigned char atomic_test_inc(atomic xdata* lock)
{
    int_disable_store();
	if(!(*lock)){
	    (*lock)++;
		int_restore();
		return 0;
    }else{
	    int_restore();
	    return 1;
    }
}
void spin_lock(atomic xdata*l)
{
    while(atomic_test_inc(l));
}

void spin_unlock(atomic xdata*l)
{
    atomic_dec(l);
}

void task_sleep(struct quene xdata** q)
{
    struct quene * new =(struct quene * ) kmalloc(sizeof(struct quene));

	spin_lock(&task_data_lock);
	new->next=*q;
	*q=new;
	new->task_p=cur_task;
	cur_task->status = 1;
    spin_unlock(&task_data_lock);
    switch_task();
}

void task_wake(struct quene xdata** q)
{
    struct quene * tmp_q;
    spin_lock(&task_data_lock);
    while(*q != NULL)
    {
	    (*q)->task_p->status=0;
		tmp_q=*q;
		*q=(*q)->next;
		kfree(tmp_q);
	}

    spin_unlock(&task_data_lock);
}
