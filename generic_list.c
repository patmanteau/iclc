/**
 *
 * generic_list.c
 *
 * A generic linked list. Idea taken from http://www.cplusplus.com/forum/general/15251/.
 *
 */

#include "generic_list.h"
#include "snippets.h"

void *el_to_data_ptr(any_el_t *el) {
    void *ptr = el;
    return ptr+2*sizeof(void*);
}

void *data_to_el_ptr(void *data) {
    return data-2*sizeof(void*);
}

unsigned long generic_list_append(any_list_t *list, void *data, unsigned int size, bool copy) {
    any_el_t *new_el = safe_malloc(sizeof(any_el_t)+size);
    new_el->next = NULL;

    void *dataptr = el_to_data_ptr(new_el);
    if (copy) {
        list->copy_fun(dataptr, data);
    } else {
        memcpy(dataptr, data, size);
    }

    if (list->size == 0) {
        list->first = new_el;
        new_el->prev = NULL;
    } else {
        list->last->next = new_el;
        new_el->prev = list->last;
    }
    list->last = new_el;
    list->size++;
    return list->size;
}

void* generic_list_first(any_list_t *list) {
    any_el_t *first_el = list->first;
    void *data = NULL;
    if (list->size > 0) {
        data = el_to_data_ptr(first_el);
    }
    return data;
}

void* generic_list_last(any_list_t *list) {
    any_el_t *last_el = list->last;
    void *data = NULL;
    if (list->size > 0) {
        data = el_to_data_ptr(last_el);
    }
    return data;
}

void* generic_list_next(void *data) {
    any_el_t *this_el = data_to_el_ptr(data);
    any_el_t *next_el = this_el->next;
    void *next_data = NULL;
    if (next_el) {
        next_data = el_to_data_ptr(next_el);
    }
    return next_data;
}

void generic_list_finalize(any_list_t *list) {
    any_el_t *el = list->first;
    
    while (el) {
        any_el_t *next_el = el->next;
        if (list->free_fun != NULL) {
            list->free_fun(el_to_data_ptr(el));
	}
        free(el);
        el = next_el;
    }
}
