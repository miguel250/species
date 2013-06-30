/*
* Hash Table
*/
#define INITIAL_SIZE (1024)
#define HASH_LEGHT (97)
#define GROWTH_FACTOR (2)

typedef enum {INTEGER, STRING, DICTIONARY} dict_type;
typedef struct dict *Dict;
typedef struct elementValue *ElementValue;

struct elementValue{
    dict_type type;
    union{
        int integer;
        char *string;
        Dict *dict;
    } value;
};


struct dict{
    int size;
    int num_elements;
    struct element **table;
    void (*add)(Dict self, char *key, ElementValue value);
    void (*delete)(Dict self, char *key);
    void (*destroy)(Dict self);
    ElementValue (*get)(Dict self, char *key); 
};


struct element {
    struct element *next;
    char *key;
    struct elementValue *data;
};

static Dict initDict(int size);
static void grow(Dict self);
static unsigned long hash_function(char *s);
Dict createDict(void);
void add(Dict self, char *key, ElementValue value);
void delete(Dict self, char *key);
void destroy(Dict self);
ElementValue get(Dict self, char *key);