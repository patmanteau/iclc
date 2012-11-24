/**
 *
 * generic_list.h
 *
 * A generic linked list. Idea taken from http://www.cplusplus.com/forum/general/15251/.
 *
 */

#ifndef _GENERIC_LIST_H_
#define _GENERIC_LIST_H_

#include <stdbool.h>

// Convenience typedef for cstrings
typedef char* char_ptr;

typedef struct any_el_tag {
    struct any_el_tag *prev;
    struct any_el_tag *next;
} any_el_t;

typedef struct any_list_tag {
    any_el_t *first;
    any_el_t *last;

    void (*copy_fun)(void*, const void*);
    int (*comp_fun)(const void*, const void*);
    void (*free_fun)(void*);

    unsigned long size;
} any_list_t;

#define DECLARE_LIST_T(TYPE, COPY_FUNP, COMP_FUNP, FREE_FUNP) \
typedef struct TYPE##_el_tag {                          \
    struct TYPE##_el_tag *prev;                         \
    struct TYPE##_el_tag *next;                         \
                                                        \
    TYPE data;                                          \
} TYPE##_el_t;                                          \
                                                        \
typedef struct TYPE##_list_tag {                        \
    TYPE##_el_t *first;                                 \
    TYPE##_el_t *last;                                  \
                                                        \
    void (*copy_fun)(void*, const void*);               \
    int (*comp_fun)(const void*, const void*);          \
    void (*free_fun)(void*);                            \
                                                        \
    unsigned long size;                                 \
} TYPE##_list_t;                                        \
                                                        \
void (*TYPE##_copy_fun)(void*, const void*) = COPY_FUNP;\
 int (*TYPE##_comp_fun)(const void*, const void*) = COMP_FUNP;  \
void (*TYPE##_free_fun)(void*) = FREE_FUNP;

#define list_t(TYPE) TYPE##_list_t

#define EMPTY_LIST(TYPE) { .first=NULL, .last=NULL, .size=0,\
.copy_fun=TYPE##_copy_fun, .comp_fun=TYPE##_comp_fun, .free_fun=TYPE##_free_fun }

#define list_append(TYPE, LIST, DATA) \
do {                                    \
    TYPE __datum = (DATA);              \
    generic_list_append(                \
        (any_list_t*)&(LIST),           \
        (void*)&(__datum),              \
        sizeof(TYPE),                   \
        false                           \
        );                              \
} while (0)

#define list_append_copy(TYPE, LIST, DATA)    \
do {                                    \
    TYPE __datum = (DATA);              \
    generic_list_append(                \
        (any_list_t*)&(LIST),           \
        (void*)&(__datum),              \
        sizeof(TYPE),                   \
        true                            \
        );                              \
} while (0)

#define list_first(TYPE, LIST) ((TYPE *)generic_list_first((any_list_t*)&(LIST)))
#define list_last(TYPE, LIST) ((TYPE *)generic_list_last((any_list_t*)&(LIST)))
#define list_next(TYPE, DATA) ((TYPE *)generic_list_next((void *)(DATA)))
#define list_finalize(LIST) (generic_list_finalize((any_list_t*)&(LIST)))

unsigned long generic_list_append(any_list_t *list, void *data, unsigned int size, bool copy);
void* generic_list_first(any_list_t *list);
void* generic_list_last(any_list_t *list);
void* generic_list_next(void *data);
void generic_list_finalize(any_list_t *list);

#endif // _GENERIC_LIST_H_

