#include "atomic.h"

#define MEM_BLOCK_BYTE 12
//#define MEM_LIST_SIZE 256 //24576B
#define MEM_START 0xa000
//#define MEM_END MEM_START+MEM_LIST_SIZE*MEM_BLOCK_BYTE*8
//#define MEM_END 0xffff
#define MEM_SIZE 0x6000



//xdata unsigned char mem_list[MEM_LIST_SIZE];
xdata atomic mem_has_initilized=0;

void * kmalloc(unsigned int bytes)
{
    unsigned int size;
    unsigned int new_size;
    unsigned char available;
    void xdata * current_location = MEM_START;

    if(!atomic_test_inc(&mem_has_initilized)){
            *(unsigned int *)current_location=MEM_SIZE;
    }
    bytes += 2; //add the block header size
    if(bytes < (unsigned int)MEM_BLOCK_BYTE){
	    bytes = MEM_BLOCK_BYTE;
	}
    size = (unsigned int)((*(unsigned int *)current_location)&0x7fff);
    available = ((*(unsigned char*)current_location)&0x10)?0:1;
    while( !available || size < (bytes + 2)){
        current_location += size;      
        if(current_location == 0)return 0;//out of mem
        size = (unsigned int)(*(unsigned int *)current_location) & 0x7fff;
        available = ((*(unsigned char*)current_location) & 0x10)?0:1;
    }
    if(( new_size = (size-bytes)) >= MEM_BLOCK_BYTE){
	    /*splite out new block*/
        *(unsigned int *)(current_location+bytes)=new_size;
        *(unsigned int *)(current_location)=bytes;
    }
	(*(unsigned int *)(current_location)) |= (0x1000);
    return (current_location + 2);
}
