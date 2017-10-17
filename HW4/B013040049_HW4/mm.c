#include <stdio.h>
#include <string.h>
#include "mm.h"
#define NALLOC 1024 /* minimum #units to request */

static Header *morecore(unsigned nu);
static Header base; /*empty list to get started*/
static Header *freep = NULL; /*start of free list*/

void *mymalloc(size_t size){
	Header *p,*prevp;
	Header *morecore(unsigned);
	unsigned nunits;

	nunits = (size+sizeof(Header)-1)/sizeof(Header)+1; /*要計算所要空間為幾塊header再+1是要加上一塊的意思,一個int的size是4bytes*/

	if((prevp=freep) == NULL){ /*現在還沒有free list*/
		base.s.ptr = freep = prevp =&base; /*base為一開始就宣告的header,這一行就是讓base的ptr指向自己*/
		base.s.size = 0; /*個header指向自己,沒有任何的記憶空間,所以為0*/
	}

	for(p=prevp->s.ptr; ; prevp = p, p = p->s.ptr){
		if(p->s.size >= nunits){ /*若有一塊空間是大於或等於我們所需要*/
			if (p->s.size == nunits){ /*如果剛好等於我們所需要,那可以直接拿來用,然後把串列連起來好就好*/
				prevp->s.ptr = p->s.ptr;
			}
			else{ /*若拿到空間比我們所需求還要大，需要切割*/
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p+1); /*回傳memory的部份就好*/
		}
		if(p == freep) /*如果找不到夠大的,就要呼叫morecore來創造,一開始prevp=freep兩個指向同個位置,繞到最後如果p==freep表示繞了一圈*/
			if((p=morecore(nunits)) == NULL)
				return NULL;
	}
}

void myfree(void *ap){

	Header *bp, *p;

	bp = (Header *)ap - 1; /*指向block的header部份*/

	for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr) /*找尋適當位置*/
		if(p >= p->s.ptr && ( bp > p || bp < p->s.ptr)) /*整個link list是按照address的大小串起來的,迴圈在比較address,找尋適當位置*/
			break;

	if(bp + bp->s.size == p->s.ptr){ /*因為連續,所以就把他們合在一起*/
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	}
	else{ /*若沒連續就串起來*/
		bp->s.ptr = p->s.ptr;
	}

	if(p + p->s.size == bp){
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	}
	else{
		p->s.ptr = bp;
    }
	freep = p;
}

void *mycalloc(size_t nmemb, size_t size){
	Header *h;
	size_t s = nmemb*size;
	h = mymalloc(s);
    if(h!=NULL){
        memset (h,0,s); /*將某一記憶體區段h的前s個字元全部設定為某一字元*/
    }
	return h;
}

void* myrealloc ( void *ptr, size_t size ){

	Header *p;

	if(ptr==NULL){ //效果等於mymalloc
		return mymalloc(size);
	}
	else if(size==0){ //效果等於free
		myfree(ptr);
		return NULL;
	}
	else{
	p = mymalloc( size ); /*要改寫的大小*/
	if( ptr != NULL ){
		Header *hPtr = (Header *)ptr - 1; /*原來*/
		Header *tempHPtr = (Header *)p -1; /*後來*/

		unsigned minSize = tempHPtr->s.size < hPtr->s.size ? tempHPtr->s.size : hPtr->s.size; /*如果後來大於原來則最小要copy的size為原來，反之*/

		memcpy( (tempHPtr+1), (hPtr+1), (minSize-1)*sizeof(Header) ); /*copy hPtr+1 到 tempHPtr+1*/
		myfree(ptr);
	}
	return p;
}
}

static Header *morecore(unsigned nu){
    /*在前面宣告的NALLOC為最小需求空間*/
    /*nu是指多少header的大小*/
	char *cp, *sbrk(int); /*sbrk是用來跟系統要空間*/
	Header *up;

	if(nu<NALLOC){ /*小於的話直接要nalloc這麼大的空間*/
        nu = NALLOC;
	}
	cp = sbrk(nu * sizeof(Header)); /*跟系統要求空間用,cp來接收*/
	if(cp== (char *) -1){ /*若還是要不到空間*/
		return NULL;
	}
	up = (Header *) cp;
	up->s.size = nu;
	myfree((void *)(up+1)); /*這裡myfree把空間放到linklist裡,+1則是要跳過新增記憶體前面header所佔用的空間*/
	return freep;
}
