#include <stdbool.h>
#include "util.h"

#define MAXLINE 1024


typedef struct key_value *KeyValue;

struct key_value{
    char *key;
    char *value;
};

ElementValue processResult(char *value);
Dict* getArrayDict(char **text, int location, int count);
void parserFile(char *fileName);
void addValue(Dict config, char *currentLine);
Dict process(char **text, int count);
bool isIndent(char *text, int next);
KeyValue getData(char *currentLine);
bool isArray(char *currentLine);
int indentationNum(char *currentLine);