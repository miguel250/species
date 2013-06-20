#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void parserFile(char *fileName)
{
    FILE *file = fopen(fileName, "r");
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        int i, len;
        char configName[250];
        char configValue[250];
        bool foundName = false;
        
        configValue[0] = '\0';
        len = strlen(line);

        for (i = 0; i < len; i++){
            
            //create key name
            if(line[i] != ':'&& !foundName){
               configName[i] = line[i];
            }else if(line[i] == ':' && !foundName){
                foundName = true;
            }

            //remove comment from files
            if(line[i] == '#'){
                break;
            }

            /*
            * Process line and get value for key
            */
            if(line[i] != '\n' && line[i] != ' ' && line[i] != ':' && foundName){
                char currentChar[1];
                currentChar[0] =  line[i];

                int size = strlen(configValue);
                configValue[size] = line[i];
                configValue[size+1] = '\0';
            }
        }

        if(configValue[0] == '\0'){
            printf("Key: %s Value: %s \n", configName, configValue);

        }
        
        memset(configValue, 0, sizeof(configValue));
        memset(configName, 0, sizeof(configName));
    }
    memset(line,0, sizeof(line));
    fclose(file);
}