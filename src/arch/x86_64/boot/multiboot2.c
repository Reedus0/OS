#include "multiboot2.h"

multiboot2_tag_t* get_tag(multiboot2_info_t* mbd, size_t type) {
    size_t i = 0;
    multiboot2_tag_t* current_tag; 
    while(i < mbd->size) {
        current_tag = &mbd->tags + i;
        if(current_tag->type == type) {
            break;
        }
        i = i + current_tag->size;
    }

    return current_tag;
}