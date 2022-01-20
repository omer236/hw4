#include <iostream>
#include <unistd.h>
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
    MallocMetadata *sbrk_res=(MallocMetadata*)sbrk(size+sizeof(MallocMetadata));
    sbrk_res->size=size;
    sbrk_res->is_free=false;
    sbrk_res->block_addr=sbrk_res;
    if(head== nullptr){
        head=sbrk_res;
    }
    if(sbrk_res==(void*)-1){
        return NULL;
    }
    return sbrk_res;
}
void* scalloc(size_t num, size_t size){

}
void sfree(void* p){
    if(head== nullptr){
        head=sbrk_res;
        return sbrk_res;
    }
    ptr=head;
    while(ptr->block_addr<=sbrk_res){
        ptr=ptr->next;
    }
    if(ptr!= nullptr){
        sbrk_res->prev=ptr->prev;
        ptr->prev->next=sbrk_res;
        ptr->prev=sbrk_res;
        sbrk_res->next=ptr;
    }
}
void* srealloc(void* oldp, size_t size){

}


size_t _num_free_blocks(){

}
size_t _num_free_bytes(){

}
size_t _num_allocated_blocks(){

}
size_t _num_allocated_bytes(){

}
size_t _num_meta_data_bytes(){

}
size_t _size_meta_data(){

}
