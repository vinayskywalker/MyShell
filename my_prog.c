#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>



char* defined_command[]={
    "cd",
    "clr",
    "dir",
    "environ",
    "echo",
    "pause",
    "help",
    "quit",
    "history"

};

int my_cd(char **args){
    return 1;
}
int my_clr(char **args){
    printf("\e[1;1H\e[2J");
    return 1;

}
int my_dir(char **args){
    return 1;
}
int my_environ(char **args){
    return 1;
}
int my_echo(char **args){
    return 1;
}
int my_pause(char **args){
    return 1;
}
int my_help(char **args){
    return 1;
}
int my_quit(char **args){
    return 1;
}
int my_history(char **args){
    return 1;
}

char** get_command(char* input){
    char *tk;
    int bufsize=1024;
    int i=0;
    char **args=(char **)malloc(bufsize*sizeof(char*));
    tk=strtok(input," ");
    while(tk!=NULL){
        // printf("%s\n",tk);
        args[i]=tk;
        i++;
        tk=strtok(NULL," ");
    }
    args[i]=tk;
    return args;
    
}

int execute_my_command(char **args){
    int i,j;
    if(args[0]==NULL){
        return 1;  // no command
    }
    int flag=1;
    int sz=sizeof(defined_command)/sizeof(char *);
    for(i=0;i<sz;i++){
        if(strcmp(args[0],defined_command[i])){
            flag=0;
            break;
        }
    }
    if(flag){
        printf("Command '%s' not found\n",args[0]);
        return 1;
    }
    if(strcmp(args[0],defined_command[0])){
        return my_cd(args);
    }
    else if(strcmp(args[0],defined_command[1])){
        return my_clr(args);
    }
    else if(strcmp(args[0],defined_command[2])){
        return my_dir(args);
    }
    else if(strcmp(args[0],defined_command[3])){
        return my_environ(args);
    }
    else if(strcmp(args[0],defined_command[4])){
        return my_echo(args);
    }
    else if(strcmp(args[0],defined_command[5])){
        return my_pause(args);
    }
    else if(strcmp(args[0],defined_command[6])){
        return my_help(args);
    }
    else if(strcmp(args[0],defined_command[7])){
        return my_quit(args);
    }
    else if(strcmp(args[0],defined_command[8])){
        return my_history(args);
    }
    else{
        printf("command '%s' not found\n",args[0]);
        return 1;
    }

}

char* read_command(){
    int bufsize=1024;
    char* buffer=(char *)malloc(sizeof(char)*bufsize);
    int i=0;
    if(!buffer){
        printf("Allocation error\n");
        exit(0);
    }
    while(1){
        char ch=fgetc(stdin);
        if(ch=='\n'){
            buffer[i]='\0';
            return buffer;
        }
        else{
            buffer[i]=ch;
        }
        i++;
    }
}

void my_helper(){
    int status;
    char *command,*input;
    char **args;
    //terminal needs to run atleast one time
    // thats why use do while
    do{
        printf("# ");
        input=read_command();
        // printf("%s\n",input);

        args=get_command(input);
        status=execute_my_command(args);

        // status=0;

    }
    while(status);
}
int main(int argc,char** argv){
    my_helper();
    return 0;
}