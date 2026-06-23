//*** Libraries ***//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//*** Defines ***//
#define SHELL_LINE_BUFSIZE 1024
#define SHELL_TOKEN_BUFSIZE 64
#define SHELL_TOKEN_DELIM " \t\r\n\a"

//*** Functions ***//

//Function for reading a line
char *shell_read_line(void){
    int bufferSize = SHELL_LINE_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufferSize);
    int c;

    if(!buffer){
        fprinf(stderr, "Shell allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        //read the char
        c = getchar();
        
        //if we get EOF
        if(c == EOF || c == '\n'){
            buffer[position] = '\0';
            return buffer;
        } else {
            buffer[position] = c;
        }
        position++;

        //if we go past the buffer size, we reallocate the memory
        if(position >= bufferSize){
            bufferSize += SHELL_LINE_BUFSIZE;
            buffer = realloc(buffer, bufferSize);
            if(!buffer){
                fprintf(stderr, "Shell allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

//Parsing function
char **shell_split_line(char *line){
    int bufferSize = SHELL_TOKEN_BUFSIZE;
    int position = 0;
    char **tokens = malloc(bufferSize * sizeof(char*));
    char* token;

    //if we could not allocate
    if(!tokens){
        fprintf(stderr, "Tokenization error\n");
        exit(EXIT_FAILURE);
    }
    //tokenize the line
    token = strtok(line, SHELL_TOKEN_DELIM);
    while (token != NULL){
        tokens[position] = token;
        position ++;
        
        //if we go past the buffer size, we reallocate the memory
        if(position >= bufferSize){
            bufferSize += SHELL_TOKEN_BUFSIZE;
            tokens = realloc(tokens, bufferSize * sizeof(char *));
                if(!tokens){
                    fprintf(stderr, "Tokenization error\n");
                    exit(EXIT_FAILURE);
                }
        }
        token = strtok(NULL, SHELL_TOKEN_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

//basic loop for input and the base of the shell
void shell_loop(void){
    char *line;
    char **args;
    int status;

    do {
        printf(">");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_execute(args);

        free(line);
        free(args);

    }while (status);
}

//main function : launches the shell
int main (int argc, char **argv){
    //for oading config files 
    //loop function 

    shell_loop();

    //cleanup for shutdown

    return EXIT_SUCCESS;
}