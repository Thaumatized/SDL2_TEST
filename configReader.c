#include <stdio.h>
#include <string.h>

#define MAX_LINE 256

enum configOutputType {
    STRING
    INT
}

typedef struct config_fetch_options {
    char name[MAX_LINE];
    void* value_ptr;
    configOutputType outputType;
} 

int getConfigurations(char* filename, config_fetch_options* options, int optionsCount)
{
    //read config.ini for values
    FILE *fp;
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open config.ini\n" );
        exit(1);
    }

    char line[MAX_LINE];
    int lineNumber = 0;
    while(fgets(line, sizeof(line), fp) != NULL)
    {
        lineOffset = 0;
        while(lineOffset < MAX_LINE && line[lineOffset] == ' ') lineOffset++;
        if(lineOffset >= MAX_LINE || line[lineOffset] == '\n' || line[lineOffset] == "#") continue;

        char name[MAX_LINE];
        char value[MAX_LINE];
        memset(name, 0, MAX_LINE);
        memset(value, 0, MAX_LINE);
        int resultOffset = 0;
        while(lineOffset < MAX_LINE && line[lineOffset] != '=')
        {
            if(line[lineOffset] == '\n')
            {
                printf("%s line %i has a problem.\n", filename, lineNumber);
                exit(1);
            }
            if(line[lineOffset] != ' ')
            {
                name[resultOffset] = line[lineOffset];
                resultOffset++;
            }
            lineOffset++;
        }
        lineOffset++;
        resultOffset = 0;
        while(lineOffset < MAX_LINE && line[lineOffset] != '\n')
        {
            if(line[lineOffset] != ' ')
            {
                value[resultOffset] = line[lineOffset];
                resultOffset++;
            }
            lineOffset++;
        }

        int matchingOptionIndex = -1;
        for(int i = 0; i < optionsCount; i++)
        {
            if(!strcmp(options[i].name, name))
            {
                matchingOptionIndex = i;
                break;
            }
        }
        
        if(i == -1)
        {
            printf("%s line %i has a problem.\n", filename, lineNumber);
        }

        switch (options[i].outputType) {
            case configOutputType.STRING:
                
        }



        lineNumber++;
    }

    fclose(fp);
}