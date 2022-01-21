#include <unistd.h>
#include <string.h>

#define MAX_SIZE 100000000


typedef struct MallocMetadata_t {
    size_t size;
    bool is_free;
    void* address;
    struct MallocMetadata_t* next;
    struct MallocMetadata_t* prev;
} MallocMetadata;


class MallocList {

private:
    MallocMetadata* head;

public:
    MallocList(): head(nullptr) {}
    ~MallocList() {}

    void* insert(size_t size) {
        MallocMetadata* meta_data = (MallocMetadata*)sbrk(sizeof(MallocMetadata));
        void* address = sbrk(size);

        if (*((int*)meta_data) == -1 || *((int*)address) == -1) {
            return nullptr;
        }

        meta_data->size = size;
        meta_data->is_free = false;
        meta_data->address = address;
        meta_data->prev = nullptr;
        meta_data->next = nullptr;

        if (head == nullptr) {
            head = meta_data;
        } else {
            if (head->address > address) {
                head->prev = meta_data;
                meta_data->next = head;
                head = meta_data;
            } else {
                MallocMetadata* current = head;

                while (current->next != nullptr &&
                       address > current->next->address) {
                    current = current->next;
                }

                if (current->next != nullptr) {
                    current->next->prev = meta_data;
                }

                MallocMetadata* tmp = current->next;
                current->next = meta_data;
                meta_data->prev = current;
                meta_data->next = tmp;
            }
        }

        return address;
    }

    void* findSmallestMatchingAddress(size_t size) {
        if (head == nullptr) {
            return nullptr;
        } else {
            MallocMetadata* current = head;

            while (current != nullptr) {
                if (current->size >= size && current->is_free) {
                    current->is_free = false;
                    return current->address;
                }

                current = current->next;
            }
        }

        return nullptr;
    }

    void free(void* address) {
        MallocMetadata* current = head;

        while (current != nullptr) {
            if (current->address == address) {
                current->is_free = true;
                break;
            }

            current = current->next;
        }
    }

    size_t findSize(void* address) {
        if (head == nullptr) {
            return -1;
        }

        MallocMetadata* current = head;

        while (current != nullptr) {
            if (current->address == address) {
                return current->size;
            }

            current = current->next;
        }

        return -1;
    }

    size_t _num_free_blocks() {
        size_t counter = 0;

        MallocMetadata* current = head;

        while (current != nullptr) {
            if (current->is_free) {
                counter++;
            }

            current = current->next;
        }

        return counter;
    }

    size_t _num_free_bytes() {
        size_t counter = 0;

        MallocMetadata* current = head;

        while (current != nullptr) {
            if (current->is_free) {
                counter += current->size;
            }

            current = current->next;
        }

        return counter;
    }

    size_t _num_allocated_blocks() {
        size_t counter = 0;

        MallocMetadata* current = head;

        while (current != nullptr) {
            counter++;
            current = current->next;
        }

        return counter;
    }

    size_t _num_allocated_bytes() {
        size_t counter = 0;

        MallocMetadata* current = head;

        while (current != nullptr) {
            counter += current->size;
            current = current->next;
        }

        return counter;
    }

    size_t _num_meta_data_bytes() {
        return this->_size_meta_data() * this->_num_allocated_blocks();
    }

    size_t _size_meta_data() {
        return sizeof(MallocMetadata);
    }

};



MallocList* malloc_list = (MallocList*)sbrk(sizeof(MallocList));

void* smalloc(size_t size) {
    if (size <= 0 || size > MAX_SIZE) {
        return NULL;
    }

    void* address = malloc_list->findSmallestMatchingAddress(size);

    if (address != nullptr) {
        return address;
    }

    return malloc_list->insert(size);
}


void* scalloc(size_t num, size_t size) {
    void* address = smalloc(num * size);

    if (address == nullptr) {
        return nullptr;
    }

    memset(address, 0, num * size);

    return address;
}

void sfree(void* p) {
    if (p != nullptr) {
        malloc_list->free(p);
    }
}

void* srealloc(void* oldp, size_t size) {
    if (size <= 0 || size > MAX_SIZE) {
        return NULL;
    }

    if (oldp == nullptr) {
        return smalloc(size);
    }

    size_t old_size = malloc_list->findSize(oldp);

    if (old_size >= size) {
        return oldp;
    }

    void* address = smalloc(size);

    if (address == nullptr) {
        return nullptr;
    }

    memcpy(address, oldp, old_size);

    sfree(oldp);

    return address;
}

size_t _num_free_blocks(){
    return malloc_list->_num_free_blocks();
}

size_t _num_free_bytes(){
    return malloc_list->_num_free_bytes();
}

size_t _num_allocated_blocks(){
    return malloc_list->_num_allocated_blocks();
}

size_t _num_allocated_bytes(){
    return malloc_list->_num_allocated_bytes();
}

size_t _num_meta_data_bytes(){
    return malloc_list->_num_meta_data_bytes();
}

size_t _size_meta_data(){
    return malloc_list->_size_meta_data();
}
