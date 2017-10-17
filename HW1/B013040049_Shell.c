#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#define SIZE 50
#define STD_INPUT 0
#define STD_OUTPUT 1
#define TRUE 1

void cd(char *pathname){ /*cd 的的fuction*/
chdir(pathname);
}

int main(void){

    while(TRUE){

        char *argv_1[SIZE]={0}, *argv_2[SIZE]={0}, *temp=NULL;
        char input[100]={0};
        int i, background=0, work;
        char *process_1, *process_2;
        int fd[2], _fd;
        pid_t pid[2], _pid; /*_pid來接fork回傳值(fork回傳值不能用一般的int來接)*/
        char *pathname;

        printf("B013040049_Shell:");
        fgets(input, 100, stdin); /*stdin標準輸入串流*/

        /*因為fgets進來他會讀到n-1個字或是\n，之後在末尾加上\0(字串結束標誌)*/

        if(input[strlen(input)-1]=='\n'){ /*取得字串長度*/
            input[strlen(input)-1]='\0';
        }

        if(strchr(input, '&')!=NULL){ /*『會搜尋到第一個＆為止*/
            char *p;
            p=strrchr(input, '&'); /*存取最後一次出現＆的位置的指標*/
            input[(int)(p-input)]='\0';
            background=1;
        }

        if(strstr(input,"cd")!=NULL){ /*cd要另外處理*/
            temp = strtok(input, " ");
            for(i=0; i<=1; i++){
                argv_1[i]=temp;
                temp=strtok(NULL, " ");
                cd(argv_1[1]);
            }
        }

        if(strchr(input, '|')!=NULL){ /*two-process*/
            work=1;
            process_1=strtok(input, "|");
            process_2=strtok(NULL, "|");

            temp=strtok(process_1, " ");
            for(i=0; i<SIZE && temp!=NULL ; i++){
                argv_1[i]=temp;
                temp=strtok(NULL, " ");
            }

            temp=strtok(process_2, " ");
            for(i=0; i<SIZE && temp!=NULL ; i++){
                argv_2[i]=temp;
                temp=strtok(NULL, " ");
            }
        }
        else if(strchr(input, '<')!=NULL){ /*input redirection，read的感覺*/
            work=2;
            process_1=strtok(input, "<");
            process_2=strtok(NULL, "<");

            temp=strtok(process_1, " ");
            for(i=0; i<SIZE && temp!=NULL ; i++){
                argv_1[i]=temp;
                temp=strtok(NULL, " ");
            }

            temp=strtok(process_2, " ");
            for(i=0; i<SIZE && temp!=NULL ; i++){
                argv_2[i]=temp;
                temp=strtok(NULL, " ");
            }
        }
        else if(strchr(input, '>')!=NULL){ /*output redirection，write的感覺*/
            work=3;
            process_1=strtok(input, ">");
            process_2=strtok(NULL, ">");

            temp=strtok(process_1, " ");
            for(i=0; i<SIZE && temp!=NULL; i++){
                argv_1[i]=temp;
                temp=strtok(NULL, " ");
            }

            temp=strtok(process_2, " ");
            for(i=0; i<SIZE && temp!=NULL; i++){
                argv_2[i]=temp;
                temp=strtok(NULL, " ");
            }
        }
        else{ /*one-process*/
            work=4;
            temp=strtok(input, " ");
            for(i=0; i<SIZE && temp!=NULL; i++){
                argv_1[i]=temp;
                temp=strtok(NULL, " ");
            }
        }

        pipe(&fd[0]); /*建立pipe，並return兩個file descriptor*/

        /*fd[0]存放供read讀使用的file descriptor，fd[1]存放供write寫使用的file descriptor*/

        if((_pid=fork())!=0){ /*創造child process，並判斷回傳值*/
            if(background==1){
                continue;
            }
            close(fd[0]); /*關閉pipe，若不關會造成死結*/
            close(fd[1]); /*關閉pipe*/
            waitpid(_pid, NULL, 0); /*阻塞，等待child process退出。pid = -1，會等待任何一個child process*/
        }
        else{
            switch(work){
                case 1: /*two-process*/
                    if((pid[0]=fork())==0){
                        close(fd[0]); /*關掉read*/
                        close(STD_OUTPUT); /*準備新的標準輸出*/
                        dup(fd[1]); /*將標準輸出指向fd[1]，這樣就使file descriptor1可被用於向管道寫*/
                        close(fd[1]); /*不需要了關閉*/
                        execvp(argv_1[0], argv_1);
                        exit(0);
                    }
                    if((pid[1]=fork())==0){
                        close(fd[1]); /*關閉write*/
                        close(STD_INPUT); /*準備新的標準輸入*/
                        dup(fd[0]); /*將標準輸入指向fd[0]，這樣就使file descriptor2可被用於向管道讀*/
                        close(fd[0]); /*不需要了關閉*/
                        execvp(argv_2[0], argv_2);
                        exit(0);
                    }
                    close(fd[0]); /*關閉pipe*/
                    close(fd[1]); /*關閉pipe*/
                    waitpid(pid[1], NULL, 0); /*pid[1]=0會等待任何一個group process ID跟parent process相同的child process離開*/
                    break;

                case 2: /*input*/
                    _fd=open(argv_2[0], O_RDONLY); /*開啟一個檔案作為標準輸入的來源*/
                    close(STD_INPUT);
                    dup2(_fd, STD_INPUT); /*會將STD_INPUT(未使用的file descriptor)指向_fd(已經打開的檔)所指向的文件*/
                    close(_fd);
                    execvp(argv_1[0], argv_1);
                    break;

                case 3: /*output*/
                    _fd=open(argv_2[0], O_WRONLY | O_CREAT | O_TRUNC, 0644); /*開啟一個檔案作為標準輸出，並將屬性設為644*/
                    close(STD_OUTPUT);
                    dup2(_fd, STD_OUTPUT); /*會將STD_OUTPUT(未使用的file descriptor)指向_fd(已經打開的檔)所指向的文件*/
                    close(_fd);
                    execvp(argv_1[0], argv_1);
                    break;

                case 4:
                    execvp(argv_1[0], argv_1); /*execvp()會從所指的目錄中查找符合參數file 的文件名，找到後便執行該文件，然後將第二個參數argv傳給該欲執行的文件。*/
            }
        }
    }
    return 0;
}
