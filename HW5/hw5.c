#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    int fd;
    char *temp;

    char input[1000];
    int mapSize = 0x1000;

    fd = open("file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    ftruncate(fd,1000);
    temp = mmap(NULL, mapSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    printf("Please input the content in to file.txt , when you input exit can leave\n");

    while(1){
        scanf("%s",input);
        strcat(temp,input);
        strcat(temp,"\n");
        if(strcmp(input,"exit")==0){
            break;
        }
    }

    munmap(NULL,mapSize);

    return 0;
}
