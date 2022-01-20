#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100000000
typedef struct MallocMetadata_t {
    size_t size;
    bool is_free;
    void* block_addr;
    struct MallocMetadata_t* next;
    struct MallocMetadata_t* prev;
} MallocMetadata;

MallocMetadata *head= nullptr;

void* smalloc(size_t size){
    MallocMetadata *ptr=head;
    if(size>MAX_SIZE||size==0){
        return NULL;
    }
    while(ptr!= nullptr){
        if(ptr->size>=size&&ptr->is_free){
            ptr->is_free=false;
            return ptr+sizeof(MallocMetadata);
        }
        ptr=ptr->next;
    }
    void *pointer_sbrk=sbrk(size+sizeof(MallocMetadata));
    MallocMetadata *sbrk_res=(MallocMetadata*)pointer_sbrk;
    sbrk_res->size=size;
    sbrk_res->is_free=false;
    sbrk_res->block_addr=pointer_sbrk;
    if(head== nullptr){
        head=sbrk_res;
        return sbrk_res;
    }
    ptr=head;
    while(ptr->block_addr<=ptr->block_addr){
        ptr=ptr->next;
    }
    if(ptr!= nullptr){
        sbrk_res->prev=ptr->prev;
        ptr->prev->next=sbrk_res;
        ptr->prev=sbrk_res;
        sbrk_res->next=ptr;
    }
    if(sbrk_res==(void*)-1){
        return NULL;
    }
    return pointer_sbrk;
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
    MallocMetadata *current=(MallocMetadata*)p;
    current->is_free= true;
    if(head== nullptr){
        head=current;
        return;
    }
    MallocMetadata *ptr=head;
    while(ptr->block_addr<=current->block_addr){
        ptr=ptr->next;
    }
    if(ptr!= nullptr){
        current->prev=ptr->prev;
        ptr->prev->next=current;
        ptr->prev=current;
        current->next=ptr;
    }
}
void* srealloc(void* oldp, size_t size){
    MallocMetadata *ptr;
    MallocMetadata *old=(MallocMetadata*)oldp;
    if(old->size>size){
        return oldp;
    }
    if(oldp==NULL){
        return smalloc(size);
    }
    ptr= (MallocMetadata*)smalloc(size);
    if(ptr==NULL){
        return NULL;
    }
    memcpy(ptr, oldp, size);
    sfree(oldp);
    return ptr;
}


size_t _num_free_blocks(){
    MallocMetadata *ptr=head;
    int count=0;
    while(ptr!=NULL){
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
    while(ptr!=NULL){
        if(ptr->is_free) {
            sum_bytes += ptr->size;
        }
        ptr=ptr->next;
    }
    return sum_bytes;
}
size_t _num_allocated_blocks(){

}
size_t _num_allocated_bytes(){

}
size_t _num_meta_data_bytes(){

}
size_t _size_meta_data(){

}
