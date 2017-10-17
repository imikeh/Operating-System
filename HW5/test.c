#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){

	int fd;
	char *temp;

	fd = open("file.txt",O_RDWR | O_CREAT , S_IRUSR | S_IWUSR);
	ftruncate(fd,1000);
	temp = mmap(NULL,1000,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

	printf("Now print the content in the file.txt , every 5 sec update once\n\n");
	sleep(5);

	while(1){
		printf("%s\n",temp);
		printf("=======================================================\n\n");
		sleep(5);
	}

	return 0;
}
