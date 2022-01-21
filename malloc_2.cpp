#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "malloc_2.h"
#define MAX_SIZE 100000000
MallocMetadata *head= nullptr;
int count_blocks=0;
long count_bytes=0;

void* smalloc(size_t size){
    MallocMetadata *ptr=head;
    if(size>MAX_SIZE||size==0){
        return NULL;
    }
    while(ptr){
        if(ptr->size>=size&&ptr->is_free){
            ptr->is_free=false;
            return ptr+1;
        }
        ptr=ptr->next;
    }
    void *pointer_sbrk=sbrk(size+sizeof(MallocMetadata));
    if(pointer_sbrk==(void*)-1){
        return NULL;
    }
    MallocMetadata *sbrk_res=(MallocMetadata*)pointer_sbrk;
    sbrk_res->size=size;
    sbrk_res->is_free=false;
    sbrk_res->block_addr=pointer_sbrk;
    count_blocks++;
    count_bytes+=size;
    return sbrk_res+1;
}
void* scalloc(size_t num, size_t size){
    MallocMetadata *ptr;
    ptr= (MallocMetadata*)smalloc(num*size);
    if(ptr==NULL){
        return NULL;
    }
    memset(ptr, 0, num*size);
    return ptr;
}
void sfree(void* p){
    MallocMetadata *current=((MallocMetadata*)p-1);
    current->is_free= true;
    if(!head){
        head=current;
        return;
    }
    MallocMetadata *ptr=head;
    while(ptr->next) {
        if (ptr->block_addr <= current->block_addr) {
            ptr = ptr->next;
        }
        else if (ptr->block_addr > current->block_addr) {
            current->prev = ptr->prev;
            ptr->prev->next = current;
            ptr->prev = current;
            current->next = ptr;
        }
    }
    current->next = ptr->next;
    current->prev = ptr;
    ptr->next = current;
}
void* srealloc(void* oldp, size_t size){
    if(size>MAX_SIZE||size==0){
        return NULL;
    }
    if(!oldp){
        return smalloc(size);
    }
    void *ptr;
    MallocMetadata *old=((MallocMetadata*)oldp-1);
    if(old->size>size){
        return oldp;
    }
    ptr=smalloc(size);
    if(ptr==NULL){
        return NULL;
    }
    memcpy(ptr, oldp, old->size);
    sfree(oldp);
    return ptr;
}


size_t _num_free_blocks(){
    MallocMetadata *ptr=head;
    int count=0;
    while(ptr){
        if(ptr->is_free) {
            count++;
        }
        ptr=ptr->next;
    }
    return count;
}
size_t _num_free_bytes(){
    MallocMetadata *ptr=head;
    int sum_bytes=0;
    while(ptr){
        if(ptr->is_free) {
            sum_bytes += ptr->size;
        }
        ptr=ptr->next;
    }
    return sum_bytes;
}
size_t _num_allocated_blocks(){
    return count_blocks;
}
size_t _num_allocated_bytes(){
    return count_bytes;
}
size_t _num_meta_data_bytes(){
    return count_blocks*sizeof(MallocMetadata);
}
size_t _size_meta_data(){
    return sizeof(MallocMetadata);
}
