#ifndef MM_H_
#define MM_H_

#include <stdlib.h>

union header {
	struct {
		union header *ptr;
		unsigned size;
	} s;
	long x; //當作使用的記憶體,使用long是因為可以撐大可以撐大headerr,因為因為x.s共用一個記憶體,所以是以大的為主
};
typedef union header Header;

//mymalloc 可以用來產生size大小的記憶體位置並回傳指標
void *mymalloc ( size_t nbytes);

//myclloc 可以產生nmemb個size大小的連續記憶體位置並回傳指標
void *mycalloc ( size_t nmemb, size_t size);

//myrealloc 可以使ptr所指到記憶體位置的大小變為size並回傳指標;
//若ptr=NULL 則產生一個size大小的記憶體位置並回傳指標
//若size=0 則釋放ptr所指向的記憶體位置並回傳NULL
void* myrealloc ( void *ptr, size_t size );

//myfree 可以釋放ptr所指向的記憶體位置
void myfree ( void *ptr);


#endif // end of #ifndef MM_H_ 
