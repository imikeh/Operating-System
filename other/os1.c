#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<string.h>
#include <sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>

#define TRUE 1
#define maxi 1024
#define STD_INPUT 0
#define STD_OUTPUT 1

char  c[maxi],*cmd1[maxi],*cmd2[maxi],*temp;
int back_run=0;

void read_command();
int check_command();
void IN_REDIRECT();
void OUT_REDIRECT();
void pipeline();

int main()
{
     int pid;

     while(TRUE){
                   int status,num1=0,t=0,i;
                    back_run = 0;
                   for(i=0;i<maxi;i++){
                            cmd1[i]=NULL;
                            cmd2[i]=NULL;
                            c[i]=0;
                   }

                   printf(">>  >>");
                   read_command();
                   t=check_command();
                   if(t!=-1){
                   if(strcmp(c,"exit\n")==0){
                            break;
                   }
                    if(t==1){
                            temp = strtok(c," \t\n");
                            cmd2[num1]=temp;
                            while ( temp!= NULL ) {
                                    num1++;
                                    temp = strtok(NULL, " \t\n");
                                    cmd2[num1] = temp;
                            }
                            t=0;
                             pid=fork();
                             if(pid!=0){
                                if(back_run==0)
                                     waitpid(pid,&status,0);
                             }
                             else if(pid==0){
                                    if(back_run!=0)
                                        setsid();
                                     execvp(cmd2[0],cmd2);
                             }
                   }
                   else if(t==2){
                            pipeline();
                            t=0;
                   }
                   else if(t==3){
                           IN_REDIRECT();
                            t=0;
                   }
                   else if(t==4){
                           OUT_REDIRECT();
                            t=0;
                   }
                   }

                             t=0;
     }
    return 0;
}

void read_command()
{
     fgets(c, maxi,stdin);
}

int check_command()
{
    int  i,a=1;

    for(i=0;i<maxi;i++){
                     if(c[i]=='|'){
                                temp = strchr(c,'|');
                                *temp = '\0';
                                *(temp-1)=0;

                                 cmd1[0] = c;
                                 temp = temp+2;
                                 cmd2[0]=temp;
                                 temp = strchr(temp,'\n');
                                 *temp = '\0';

                                 a=2;
                                 return a;
                     }

                     if(c[i]=='<'){
                                temp = strchr(c,'<');
                                *temp = '\0';
                                *(temp-1)=0;
                                 cmd1[0] = c;

                                 temp = temp+2;
                                 cmd2[0]=temp;
                                 temp = strchr(temp,'\n');
                                 *temp = '\0';

                                 a=3;
                                 return a;
                     }

                     if(c[i]=='>'){
                                temp = strchr(c,'>');
                                *temp = '\0';
                                *(temp-1)=0;
                                 cmd1[0] = c;

                                 temp = temp+2;
                                 cmd2[0]=temp;
                                 temp = strchr(temp,'\n');
                                 *temp = '\0';

                                 a=4;
                                 return a;
                     }
                      if(c[i]=='&'){
                                temp = strchr(c,'&');
                                *temp = '\0';
                                *(temp-1)=0;
                                 cmd1[0] = c;

                                back_run=1;
                      }

 }

 if(a==1){
    return a;
 }
return -1;
}

void pipeline()
{
    int pid,pid2,fd,fd2;

    pid=fork();
    if(pid==0){
                pid2=fork();
                if(pid2==0){
                    fd2=open("tempfile",O_WRONLY|O_CREAT|O_TRUNC,0666);
                    dup2(fd2,STDOUT_FILENO);
                    execvp(cmd1[0],cmd1);
                }
                else{
                    waitpid(pid2,NULL,0);
                    fd=open("tempfile",O_RDONLY);
                    dup2(fd,STDIN_FILENO);
                    execvp(cmd2[0],cmd2);
                }
     }
waitpid(pid,NULL,0);
}

void IN_REDIRECT()
{
     int pid,fd;

    pid=fork();
    if(pid<0){
        printf("fork error\n");
        exit(0);
    }
    if(pid==0){
         fd=open(cmd2[0],O_RDONLY);
         dup2(fd,STDIN_FILENO);
         execvp(cmd1[0],cmd1);
    }
waitpid(pid,NULL,0);
}


void OUT_REDIRECT()
{
    int pid,fd;

    pid=fork();
    if(pid<0){
        printf("fork error\n");
        exit(0);
    }
    if(pid==0){
        fd=open(cmd2[0],O_WRONLY|O_CREAT|O_TRUNC,00666);
        dup2(fd,STDOUT_FILENO);
        execvp(cmd1[0],cmd1);
    }
waitpid(pid,NULL,0);
}
