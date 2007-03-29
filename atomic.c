#include "atomic.h"
#include "at89x51.h"
#include "gernel.h"
extern xdata unsigned char cur_task_index;
extern xdata struct task task_list[4];
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

void switch_task();
/*
void atomic_inc(atomic * s)
{
    CY=EA;
	EA=0;
	s++;
	EA=CY;
} */

void atomic_dec(atomic * s)
{
    int_disable_store();
	(*s)--;
	int_restore();
}

unsigned char atomic_test_inc(atomic * lock)
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
void spin_lock(atomic*l)
{
    while(atomic_test_inc(l));
}

void spin_unlock(atomic*l)
{
    atomic_dec(l);
}

void task_sleep(unsigned char * q)
{
    unsigned char i=0;
    spin_lock(&task_data_lock);
    while(q[i]!=0)i++;
    q[i]=cur_task_index+1;
    task_list[cur_task_index].status=1;
    spin_unlock(&task_data_lock);
    switch_task();
}

void task_wake(unsigned char * q)
{
    unsigned char i=0;
    spin_lock(&task_data_lock);
	for(i=0;i<MAX_TASK;i++){
        if(q[i]!=0){
	        task_list[q[i]-1].status=0;
		    q[i]=0;
		}
	}
    spin_unlock(&task_data_lock);
}
