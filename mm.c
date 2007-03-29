#include "atomic.h"

#define MEM_BLOCK_BYTE 12
//#define MEM_LIST_SIZE 256 //24576B
#define MEM_START 0xa000
//#define MEM_END MEM_START+MEM_LIST_SIZE*MEM_BLOCK_BYTE*8
//#define MEM_END 0xffff
#define MEM_SIZE 0x6000



//xdata unsigned char mem_list[MEM_LIST_SIZE];
xdata atomic mem_has_initilized = 0;
xdata atomic mem_being_kmalloc = 0;

void * kmalloc(unsigned int bytes)
{
    unsigned int size,new_size,next_size;
    unsigned char available, next_available;
    void xdata * current_location = MEM_START, * next_location;

    if(!atomic_test_inc(&mem_has_initilized)){
            *(unsigned int *)current_location=MEM_SIZE;
    }
    bytes += 2; //add the block header size
    if(bytes < (unsigned int)MEM_BLOCK_BYTE){
	    bytes = MEM_BLOCK_BYTE;
	}

	spin_lock(&mem_being_kmalloc);
    do{
        size = (unsigned int)((*(unsigned int *)current_location)&0x7fff);
        available = ((*(unsigned char*)current_location)&0x10)?0:1;
		if(available){
			do{//connect all later available block to one big block
				next_location = current_location + size;
				next_available = ((*(unsigned char*)next_location)&0x10)?0:1;
				if(!next_available){
				    *(unsigned int *)(current_location)= size;
					break;
				}
				size += *((unsigned int *)next_location);
			}while(1);
			if(size > bytes)break;//we find it
		}
 		current_location += size;
	    if(current_location == 0)return 0;//out of mem
    }while(1);
    if(( new_size = (size - bytes)) >= MEM_BLOCK_BYTE){
	    /*splite out new block*/
        *(unsigned int *)(current_location + bytes)= new_size; //new block can be used
        *(unsigned int *)(current_location)= (0x8000 + bytes); //used
    }
	spin_unlock(&mem_being_kmalloc);

    return (current_location + 2);
}

void kfree(void * p)
{
    p -= 2;
    *((unsigned int *)p) &= (0x7fff);
}
