#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void parserFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char line[MAXLINE];
    char *yml_text[MAXLINE];
    int count = 0;


    while (fgets(line, sizeof(line), file)) {
        yml_text[count] =  malloc(strlen(line) + 1);
        strcpy(yml_text[count], line);
        count++;
    }

    Dict config;
    config = process(yml_text, count);
    ElementValue species = config->get(config, "species");
    Dict *species_dict = species->value.dict;
    ElementValue test_dict = species_dict[0]->get(species_dict[0], "name");
    printf("%s\n", test_dict->value.string);

    config->destroy(config);

    fclose(file);
}

Dict process(char **text, int count){

    int i = 0;
    int level;
    char *currentLine;
    Dict ymlDict = createDict();
    
    for (; i < count; ++i){

        if(text[i] != NULL){
            int indent = indentationNum(text[i]);

            if(indent == 0){
                KeyValue data = getData(text[i]);
                
                if(data->value[0] == '\0'){
                    Dict *array  = getArrayDict(text, i, count);
                    ElementValue dictionary = malloc(sizeof(ElementValue)*2);
                    dictionary->type = DICTIONARY;
                    dictionary->value.dict = array;
                    ymlDict->add(ymlDict, data->key, dictionary);
                }else{
                    
                    ElementValue result = processResult(data->value);
                    ymlDict->add(ymlDict, data->key, result);
                }
            } 
        }
    }

    return ymlDict;
}

bool isArray(char *currentLine){
    int i = 0;
    char text = '-';

    for (; i < strlen(currentLine); ++i){
        if(currentLine[i] == text){
            return true;
        }
    }

    return false;
}


Dict* getArrayDict(char **text, int location, int count){
    int i = location;
    int key = 0;
    int current_key;
    Dict array[MAXLINE] = {0};
    char **tmp_text = malloc(MAXLINE*2);
    memcpy(tmp_text, text, MAXLINE);
    
    while(true){
        i++;

        if(i >= count){
            break;
        }

        int current_indent = indentationNum(tmp_text[i]);
        if(current_indent != 0 && i < count){

            if(tmp_text[i] != " "){
                bool is_array = isArray(tmp_text[i]);
                
                if(is_array){
                    array[key] = createDict();
                    current_key = key;
                    key++;
                }

                KeyValue data = getData(tmp_text[i]);

                
                if(data->value[0] == '\0'){
                    int position = i+1;
                    int next_indent = indentationNum(tmp_text[position]);
                    
                    ElementValue dictionary_el = malloc(sizeof(ElementValue)*2);
                    dictionary_el->type = DICTIONARY;
                    Dict dictionary = createDict();
                    
                    while(next_indent > current_indent){
                        KeyValue next_data = getData(tmp_text[position]);
                        ElementValue result = processResult(next_data->value);
                        dictionary->add(dictionary, next_data->key, result);
                        tmp_text[position] = " ";
                        position++;

                        if(position >= count){
                            break;
                        }
                        next_indent = indentationNum(tmp_text[position]);
                    }
                    dictionary_el->value.dict =  malloc(sizeof(Dict)+1);
                    dictionary_el->value.dict =  dictionary;
                    array[current_key]->add(array[current_key], data->key, dictionary_el);
                }else{
                    ElementValue result = processResult(data->value);
                    array[current_key]->add(array[current_key], data->key, result);
                }

            }else{
                continue;
            }
        }else{
            break;
        }
    }

    //free(tmp_text);

    return array;
}

ElementValue processResult(char *value){
    ElementValue result = malloc(sizeof(ElementValue)*2);
    if(isdigit(value[0])){
        result->type = INTEGER;
        result->value.integer = atoi(value);
    }else{
        result->type = STRING;
        result->value.string  =  malloc(strlen(value)+1);
        result->value.string = value;
    }
    return result;
}

KeyValue getData(char *currentLine){
    int i = 0;
    int len = strlen(currentLine);
    char text = ':';
    char key[250] = {0};
    char value[250]= {0};
    bool foundName = false;
    KeyValue data;
    
    data =  malloc(sizeof(KeyValue) * 2);

    for (i = 0; i < len; i++){
        //remove dash
        if(currentLine[i] == '-'){
            continue;
        }

        //trim spaces
        if(currentLine[i] == ' '){
            continue;
        }

        //create key name
        if(currentLine[i] != ':'&& !foundName){
            int key_size = strlen(key);
            key[key_size] = currentLine[i];
            key[key_size+1] = '\0';
        }else if(currentLine[i] == ':' && !foundName){
            foundName = true;
        }
        //remove comment from files
        if(currentLine[i] == '#'){
            break;
        }

        /*
        * Process line and get value for key
        */
        if(currentLine[i] != '\n' && currentLine[i] != ' ' && currentLine[i] != ':' && foundName){
            char currentChar[1];
            currentChar[0] =  currentLine[i];

            int size = strlen(value);
            value[size] = currentLine[i];
            value[size+1] = '\0';
        }

    }

    data->key = strdup(key);
    data->value =  strdup(value);
    return data;
}

int indentationNum(char *currentLine){
    int i = 0;
    int numIndent = 0;
    char space = ' ';
    char array = '-';

    for (; i < strlen(currentLine); ++i)
    {
        if(currentLine[i] == ':'){
            break;
        }

        if(currentLine[i] == space || currentLine[i] == array){
            numIndent++;
        }
    }

    return numIndent;
}