//
// Created by student on 1/21/22.
//

#ifndef MALLOC_2_MALLOC_2_H
#define MALLOC_2_MALLOC_2_H
typedef struct MallocMetadata_t {
    size_t size;
    bool is_free;
    void* block_addr;
    struct MallocMetadata_t* next;
    struct MallocMetadata_t* prev;
} MallocMetadata;


void* smalloc(size_t size);
void* scalloc(size_t num, size_t size);
void sfree(void* p);
void* srealloc(void* oldp, size_t size);

size_t _num_free_blocks();
size_t _num_free_bytes();
size_t _num_allocated_blocks();
size_t _num_allocated_bytes();
size_t _num_meta_data_bytes();
size_t _size_meta_data();
#endif //MALLOC_2_MALLOC_2_H
