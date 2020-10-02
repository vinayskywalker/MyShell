#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include<iostream>
#include <vector>
#include <list> 
// #include<bits/stdc++.h> 
using namespace std; 
list <vector<string>> history;
extern char **environ;

const char *defined_command[]={
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
    if(args[1]==NULL){
        return 1;
    }
    // int i=1;
    char *argument=(char *)malloc(sizeof(args));
    // char *path=args[1];
    string next=args[1];
    int j=2;
    while(args[j]!=NULL){
        next=next + " " + args[j];
        j++;
    }
    char path[next.size()+1];
    for(int k=0;k<next.size();k++){
        path[k]=next[k];
    }
    path[next.size()]='\0';
    int p=chdir(path);
    if(p==-1){
        printf("cd: The directory %s does not exist \n",path);
    }
    else{
       string prev=getenv("PWD");
       // cout<<prev<<endl<<next<<endl;
       int stop=prev.size()-1;
       int start=0;
        for(int i=0;next.size()>i;i++){
            if(next[i]=='.' && next[i+1]=='.'){
                for(int k=stop;k>=0;k--){
                  if(prev[k]=='/'){
                    stop=k-1;
                    break;
                  }
                }
                start=i+3;
                i=start-1;
            }
            else if(next[i]=='.' && next[i+1]=='/'){
                start=i+2;
                i=start-1;
            }

        }
        char pwd[stop+next.size()-start+2];
        for(int k=0;k<=stop;k++){
            pwd[k]=prev[k];
            if(k==stop){
                pwd[++k]='/';
                for(int j=start;j<next.size();j++){
                    k++;
                    pwd[k]=next[j];
                }
                pwd[k+1]='\0';
            }
        }
        setenv("PWD",pwd,1);
    }
    return 1;
}
int my_clr(char **args){
    printf("\e[1;1H\e[2J");
    return 1;
}
int my_dir(char **args){
    DIR *dir1;
    struct dirent *dir2;
    dir1=opendir(".");
    if(dir1){
        while((dir2=readdir(dir1))!=NULL){
            printf("%s\n",dir2->d_name);
        }
        closedir(dir1);
    }
    else{
        printf("Error\n");
    }
    return 1;
}
int my_environ(char **args){
    // cout<<getenv(args[1])<<endl;
    for(char **current=environ;*current;current++){
        cout<<(*current)<<endl;
    }
    return 1;
}
int my_echo(char **args){
    if(args[1]==NULL){
        printf("\n");
        return 1;
    }
    int i=1;
    while(args[i]!=NULL){
        printf("%s ",args[i]);
        i++;
    }
    printf("\n");
    return 1;
}
int my_pause(char **args){
    printf("Shell Paused\n");
    while(1){
        if(fgetc(stdin)=='\n'){
            break;
        }
    }
    return 1;
}
int my_help(char **args){
    if(args[1]==NULL){
        printf("These are the command Available\n");
        printf("cd\n");
        printf("clr\n");
        printf("dir\n");
        printf("environ\n");
        printf("echo\n");
        printf("pause\n");
        printf("help\n");
        printf("quit\n");
        printf("history\n");
        return 1;
    }
    if(!strcmp(args[1],"cd")){
        printf("cd - Change the shell working directory.\n");
        return 1;
    }
    else if(!strcmp(args[1],"clr")){
        printf("clr - Clear the Screen.\n");
        return 1;
    }
    else if(!strcmp(args[1],"dir")){
        printf("dir <directory> - list directory contents\n");
        return 1;
    }
    else if(!strcmp(args[1],"environ")){
        printf("environ - List all the environment strings of current shell and bash shell.\n");
        return 1;
    }
    else if(!strcmp(args[1],"echo")){
        printf("echo <comment> - Display <comment> on the screen followed by a new line.\n");
        return 1;
    }
    else if(!strcmp(args[1],"pause")){
        printf("pause - Pause Operation of shell until ‘Enter’ is pressed.\n");
        return 1;
    }
    else if(!strcmp(args[1],"quit")){
        printf("quit - Quit the shell.\n");
        return 1;
    }
    else if(!strcmp(args[1],"help")){
        printf("help - Display User Manual.\n");
        return 1;
    }
    else if(!strcmp(args[1],"history")){
        printf("history - Display a list of previously executed commands.\n");
        return 1;
    }
    else{
        printf("Command '%s' not found\n",args[1]);
        return 1;
    }

}
int my_quit(char **args){
    return 0;
}
int my_history(char **args){
     int i=0;
    for (auto it = history.begin(); it != history.end(); ++it){
            cout<<++i;
            for(auto it1 = it->begin(); it1 != it->end(); ++it1){
                cout<<" "<<*it1<<" ";
            }
            cout<<endl;
    }      
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
        if(!strcmp(args[0],defined_command[i])){
            // printf("i am in loop %s\n",args[0]);
            flag=0;
            break;
        }
    }
    // printf("%s\n",args[0]);
    if(flag){
        printf("Command '%s' not found\n",args[0]);
        return 1;
    }
    if(!strcmp(args[0],defined_command[0])){
        return my_cd(args);
    }
    else if(!strcmp(args[0],defined_command[1])){
        return my_clr(args);
    }
    else if(!strcmp(args[0],defined_command[2])){
        return my_dir(args);
    }
    else if(!strcmp(args[0],defined_command[3])){
        return my_environ(args);
    }
    else if(!strcmp(args[0],defined_command[4])){
        return my_echo(args);
    }
    else if(!strcmp(args[0],defined_command[5])){
        return my_pause(args);
    }
    else if(!strcmp(args[0],defined_command[6])){
        return my_help(args);
    }
    else if(!strcmp(args[0],defined_command[7])){
        return my_quit(args);
    }
    else if(!strcmp(args[0],defined_command[8])){
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
        cout<<"user@sp:~";
        cout<<get_current_dir_name();
        printf("~# ");
        input=read_command();
        // printf("%s\n",input);

        args=get_command(input);
        int i,sz=sizeof(args);

        int j=0;
        vector<string> temp;
        if(args[0]!=NULL)
        {
            while(args[j]!=NULL){
                temp.push_back(args[j]);
                j++;
            }
            if(history.size()==100)
                history.pop_front();
            history.push_back(temp);

        }
        status=execute_my_command(args);

        // status=0;

    }
    while(status);
}
int main(int argc,char** argv){
    if(argc==1){
        my_helper();
    }
    else if(argc==2){
        FILE *fp;
        fp=fopen(argv[1],"r");
        if(fp==NULL){
            cout<<"Can not open this file\n";
            return 0;
        }
        char **args;
        char ch;
        char mycmd[50];
        

    }
    else{
        printf("More than two command line argument is not accepted\n");
    }
    return 0;
}