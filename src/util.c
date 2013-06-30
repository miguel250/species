#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Hash Table
 * Alloc memory for dictionary
 */
static Dict initDict(int size){
    Dict dictionary = malloc(size * sizeof(Dict));
    int i;

    dictionary->add = add;
    dictionary->get = get;
    dictionary->delete = delete;
    dictionary->destroy = destroy;

    dictionary->size = size;
    dictionary->num_elements = 0;

    dictionary->table = malloc(sizeof(struct element *) * dictionary->size);
    for(i = 0; i < dictionary->size; i++){
        dictionary->table[i] = NULL;
    }
    return dictionary;
}


/*
 * Initial dictionary with default allocated memory
 */
Dict createDict(void){
    return initDict(INITIAL_SIZE);
}


/*
 * Increase dictionary size if running out of space
 */
static void grow(Dict self){
    int i, newSize;
    Dict dictionary;
    struct dict swap;
    struct element *el;

    newSize = self->size * GROWTH_FACTOR;

    dictionary = initDict(newSize);

    for (i = 0; i < self->size; ++i)
    {
        for(el = self->table[i]; el != 0; el = el->next) {
            dictionary->add(dictionary, el->key, el->data);
        }
    }

    swap = *self;
    *self = *dictionary;
    dictionary->destroy(dictionary);
}

/* 
 * Hashing algorithm 
 * http://www.cse.yorku.ca/~oz/hash.html
 */
static unsigned long hashFunction(char* str) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++){
         hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

/*
 *
 */
void add(Dict self, char *key, ElementValue value){
    struct element *el = malloc(sizeof(struct element));
    unsigned long hash;
    el->key = key;
    el->data = value;

    hash = hashFunction(key) % self->size;
    el->next = self->table[hash];
    self->table[hash] = el;
    self->num_elements++;

    if(self->num_elements >= self->size) {
       grow(self);
    }
}

/*
 *
 */
ElementValue get(Dict self, char *key){
    unsigned long hash;
    struct element *el;
    hash = hashFunction(key) % self->size;

    for (el = self->table[hash]; el != 0; el = el->next)
    {
         if(!strcmp(el->key, key)) {
            return el->data;
         }
    }
    return 0;
}

/*
 * Dealloc memory in key 
 */
void delete(Dict self, char *key){
    unsigned long hash;
    struct element *el;
    struct element **prev;
    hash = hashFunction(key) % self->size;


    for (prev = &(self->table[hash]); prev != 0; prev = &((*prev)->next))
    {
         if(!strcmp(el->key, key)) {
            el = *prev;
            *prev = el->next;

            free(el->data);
            free(el);
         }
    }
}

/*
 * Dealloc all memory use in dictionary
 */
void destroy(Dict self){
    int i;
    struct element *el;
    struct element *next;
    ElementValue data;

    for (i = 0; i < self->size; ++i)
    {
        for(el = self->table[i]; el != 0;) {
            next = el->next;
            free(el->data);
            free(el);
            el = next;
        }
    }

    free(self->table);
    free(self);
}