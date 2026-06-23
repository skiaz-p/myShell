//*** Libraries ***//
#include <stdio.h>
#include <stdlib.h>

//*** Defines ***//
#define SHELL_LINE_BUFSIZE 1024


//*** Functions ***//

//Function for reading a line
char *shell_read_line(void){
    int bufferSize = SHELL_LINE_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufferSize);
    int c;

    if(!buffer){
        fprinf(stderr, "Shell allocation error");
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
                fprintf(stderr, "Shell allocation error");
                exit(EXIT_FAILURE);
            }
        }
    }
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