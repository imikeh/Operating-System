#include <stdio.h>
#include <string.h>
//#include <stdlib.h>
#include "mm.h"

#define X 10

int main(){
	int i,*a,*b,*c,*temp;
	
	printf ("\n=== 測試mymalloc & myfree ===\n");
	printf ("( 應印出a(address)=5兩次 )\n");
	a=mymalloc(sizeof(int));
	*a=5;
	printf ("a(%p)=%d\n",a,*a);
	myfree(a);
	a=mymalloc(sizeof(int));
	printf ("a(%p)=%d\n",a,*a);
	
	printf ("\n=== 測試myfree(NULL) ===\n");
	myfree(NULL);
	
	printf ("\n=== 測試mycalloc ===\n");
	b=mycalloc (X,sizeof(int));
	printf ("( 應印出b[0~9](address)=0 )\n");
	for (i=0;i<X;i++)
		printf ("b[%d](%p)=%d\n",i,b+i,*(b+i));
	printf ("\n");
	for (i=0;i<X;i++)
		*(b+i)=i+1;
	printf ("( 應印出b[0~9](address)=1~10 )\n");
	for (i=0;i<X;i++)
		printf ("b[%d](%p)=%d\n",i,b+i,*(b+i));
	printf ("\n");

	myfree(b);
	b=mycalloc (X,sizeof(int));
	printf ("( 應印出b[0~9](address)=0 )\n");
	for (i=0;i<X;i++)
		printf ("b[%d](%p)=%d\n",i,b+i,*(b+i));
	printf ("\n");
	
	printf ("\n=== 測試realloc ===\n");
	b=myrealloc(b,0);
	printf ("( 應印出b (nil) )\n");
	printf ("b %p\n\n",b);
	
	b=myrealloc(NULL,10*sizeof(int));
	
	printf ("( 應印出size=%ld )\n",(sizeof(int)*10+sizeof(Header)-1)/sizeof(Header)+1);
	printf ("size=%d\n\n",((Header*)b-1)->s.size);

	temp=b;
	for (i=0;i<X;i++)
		*(b+i)=i+1;
	printf ("( 應印出b[0~9](address)=1~10 )\n");
	for (i=0;i<X;i++)
		printf ("b[%d](%p)=%d\n",i,b+i,*(b+i));
	printf ("\n");
	
	printf ("\n=== 測試realloc( 新size < 舊size ) ===\n");

	b=myrealloc(b,8*sizeof(int));
	printf ("( 應印出size=%ld )\n",(sizeof(int)*8+sizeof(Header)-1)/sizeof(Header)+1);
	printf ("size=%d\n\n",((Header*)b-1)->s.size);
	printf ("( 應印出b(%p) )\n",temp);
	printf ("b(%p)\n\n",b);
	printf ("( 應印出b[0~7](address)=1~8 )\n");
	for (i=0;i<8;i++)
		printf ("b[%d](%p)=%d\n",i,b+i,*(b+i));
	printf ("\n");

	printf ("\n=== 測試realloc( 新size > 舊size ) ===\n");
	b=myrealloc(b,10*sizeof(int));
	printf ("( 應印出size=%ld )\n",(sizeof(int)*10+sizeof(Header)-1)/sizeof(Header)+1);
	printf ("size=%d\n\n",((Header*)b-1)->s.size);
	printf ("( \"通常\"應印出b(%p) )\n",temp);
	printf ("b(%p)\n\n",b);
	b[8]=9;	
	b[9]=10;
	printf ("( 應印出b[0~9](address)=1~10 )\n");
	for (i=0;i<10;i++)
		printf ("b[%d](%p)=%d\n",i,b+i,*(b+i));
	printf ("\n");

	printf ("\n=== 測試realloc( 新size >> 舊size ) ===\n");
	b=myrealloc(b,30*sizeof(int));
	printf ("( 應印出size=%ld )\n",(sizeof(int)*30+sizeof(Header)-1)/sizeof(Header)+1);
	printf ("size=%d\n\n",((Header*)b-1)->s.size);
	printf ("( \"通常不會\"印出b(%p) )\n",temp);
	printf ("b(%p)\n\n",b);
	printf ("( 應印出b[0~9](address)=1~10 )\n");
	for (i=0;i<10;i++)
		printf ("b[%d](%p)=%d\n",i,b+i,*(b+i));
	printf ("\n");

	myfree(b);
	a=myrealloc(NULL,8*sizeof(int));
	b=myrealloc(NULL,16*sizeof(int));
	temp=myrealloc(NULL,30*sizeof(int));
	myfree(a);
	b=myrealloc(b,12*sizeof(int));
	c=myrealloc(c,3*sizeof(int));
	a=myrealloc(NULL,20*sizeof(int));
	temp=myrealloc(temp,0);
	b=myrealloc(b,40*sizeof(int));
	temp=myrealloc(temp,12*sizeof(int));
	b=myrealloc(b,0);
	a=myrealloc(a,0);
	c=myrealloc(c,0);
	temp=myrealloc(temp,0);
	
	a=myrealloc(NULL,8*sizeof(int));
	b=myrealloc(NULL,16*sizeof(int));
	temp=myrealloc(NULL,30*sizeof(int));
	myfree(a);
	b=myrealloc(b,12*sizeof(int));
	c=myrealloc(c,3*sizeof(int));
	a=myrealloc(NULL,20*sizeof(int));
	temp=myrealloc(temp,0);
	b=myrealloc(b,40*sizeof(int));
	temp=myrealloc(temp,12*sizeof(int));
	b=myrealloc(b,0);
	a=myrealloc(a,0);
	c=myrealloc(c,0);
	temp=myrealloc(temp,0);
	
	a=myrealloc(NULL,8*sizeof(int));
	b=myrealloc(NULL,16*sizeof(int));
	temp=myrealloc(NULL,30*sizeof(int));
	myfree(a);
	b=myrealloc(b,12*sizeof(int));
	c=myrealloc(c,3*sizeof(int));
	a=myrealloc(NULL,20*sizeof(int));
	temp=myrealloc(temp,0);
	b=myrealloc(b,40*sizeof(int));
	temp=myrealloc(temp,12*sizeof(int));
	b=myrealloc(b,0);
	a=myrealloc(a,0);
	c=myrealloc(c,0);
	temp=myrealloc(temp,0);


	return 0;
}
