//*** Libraries ***//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//*** Defines ***//
#define SHELL_LINE_BUFSIZE 1024
#define SHELL_TOKEN_BUFSIZE 64
#define SHELL_TOKEN_DELIM " \t\r\n\a"


//*** Builtin basics functions ***//

//declarations 
int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

//commands

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char**) ={
    &shell_cd,
    &shell_help,
    &shell_exit
};

int shell_num_builtins(){
    return sizeof(builtin_str) /sizeof(char *);
}

//implementations

int shell_cd(char **args){
    if(args[1] == NULL){
        fprintf(stderr, "Shell : expected argument to \"cd\"\n");
    }else{
        if(chdir(args[1]) != 0){
            perror("Shell");
        }
    }
    return 1;
}

int shell_help(char **args){
    int i;

    printf("Luc Casassus Latrille shell\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i=0; i< shell_num_builtins(); i++){
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int shell_exit(char **agrs){
    return 0;
}

//*** Kernel Functions ***//

int shell_launch(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0){
        //the child process
        if(execvp(args[0], args) == -1){
            perror("shell");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 1){
        perror("Forking error");
    }else {
        //parent process
        do{
            wpid = waitpid(pid, &status, WUNTRACED);
        }while(WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;

}

int shell_execute(char **args){
    int i;
    if(args[0] == NULL){
        return 1;
    }

    for(i=0 ; i< shell_num_builtins(); i++){
        if(strcmp(args[0], builtin_str[i]) == 0){
            return (*builtin_func[i])(args);
        }
    }

    return shell_launch(args);
}

//Function for reading a line
char *shell_read_line(void){
    int bufferSize = SHELL_LINE_BUFSIZE;
    int position = 0;
    char *buffer = malloc(sizeof(char) * bufferSize);
    int c;

    if(!buffer){
        fprintf(stderr, "Shell allocation error\n");
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