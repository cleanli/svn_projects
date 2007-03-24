#include "atomic.h"
#include "at89x51.h"
#include "gernel.h"
extern xdata unsigned char cur_task_index;
extern xdata struct task task_list[4];

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
    bit b;
    b=EA;
	EA=0;
	(*s)--;
	EA=b;
}

unsigned char atomic_test_inc(atomic * lock)
{
    bit b;
    b=EA;
	EA=0;
	if(!(*lock)){
	    (*lock)++;
		EA=b;
		return 0;
    }else{
	    EA=b;
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
    while(q[i]!=0)i++;
    q[i]=cur_task_index+1;
    task_list[cur_task_index].status=1;
    switch_task();
}

void task_wake(unsigned char * q)
{
    unsigned char i=0;
	for(i=0;i<4;i++){
        if(q[i]!=0){
	        task_list[q[i]-1].status=0;
		    q[i]=0;
		}
	}
}
