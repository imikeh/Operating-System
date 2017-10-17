#include <stdio.h>
#include "mm.h"

#define NALLOC 1024

static Header *morecore(unsigned nu);
static Header base;
static Header *freep = NULL;
static int sum=0;

//---------------------------------------------------------------------------------------------
void *mymalloc(size_t nbytes)
{
	Header *p,*prevp;
	Header *morecore(unsigned);
	unsigned nunits;
	//printf ("nbytes=%d sizeof(Header)=%d\n",nbytes,sizeof(Header));
	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header)+1;//算出所要的空間要幾塊header.+1是要加上一塊的意思
	//printf ("nunits=%d\n",nunits);

	if ((prevp = freep) == NULL )
       	{
		base.s.ptr = freep = prevp =&base;//base是一開始就有宣告的一塊Header.然後這一整個的動作就是讓base的ptr指向自己（形成cycle）
		base.s.size = 0;                  //大概初始化的情形.一個Header指向自己.沒有任何的記憶空間
	}
	for (p=prevp->s.ptr; ; prevp = p, p = p->s.ptr)//在cycle找記憶體
	{
		if (p->s.size >= nunits) //找到一塊空間大於等於需求的
		{
			if (p->s.size == nunits)//如果剛好等於.就直接拿來用.然後把串列連好就好
				prevp->s.ptr = p->s.ptr;
			else//拿到比較大的.就要把他作切割
			{
				p->s.size -= nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep = prevp;
			return (void *)(p+1);//然後回傳memory的部份就好
		}
		if (p == freep)//如果繞一圈後找不到夠大的.就叫morecore創一塊.一開始prevp = freep兩個指向同個位置.繞繞道最後如果p == freep表示繞了一圈
			if ((p=morecore(nunits)) == NULL)
				return NULL;
	}
}
//---------------------------------------------------------------------------------------------
void *mycalloc ( size_t nmemb, size_t size)//只是拿來malloc
{
	Header *p;
	int count;	
	
	p=mymalloc( nmemb * size );
	if (p!=NULL)
		for ( count=0 ; count < nmemb * size ; count++ )
		{
			*((char*)p+count)=(char)0;
		}
			
	return p;
}
//---------------------------------------------------------------------------------------------
void* myrealloc ( void *ptr, size_t size )//是用來調整空間用的
{
	Header *p,*bp;
	unsigned i,nunits;

	if (ptr==NULL)//效果等於mymalloc
		return mymalloc(size);
	else if ( size==0 )//效果等於free
	{
		myfree(ptr);
		return NULL;
	}
	else {
		nunits = (size+sizeof(Header)-1)/sizeof(Header)+1;//也是看要 要多少塊
		bp=(Header*)ptr-1;//讓bp指向目前這塊空間的Header
		
		if (nunits < bp->s.size){			//if old size < new size //如果新要的比現在還小的話.就切一切還回去	
			p=bp+nunits;
			p->s.size=bp->s.size-nunits;
			bp->s.size=nunits;			//old size = new size
			myfree(p+1);				//free (remainder size)
			return (void*)(bp+1);
		}
		else if (nunits == bp->s.size)			//if old size == new size
			return (void*)(bp+1);
								//if old size < new size
		for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)//會找出在link-list中.在bp的前一塊在哪
			if (p >= p->s.ptr && ( bp > p || bp < p->s.ptr))
				break;
		if (bp + bp->s.size == p->s.ptr && bp->s.size+p->s.ptr->s.size >= nunits){ //下一塊要+目前的>新要的空間
		//if bp and its behind free space are continued && their size sum >= new size
			if ( bp->s.size+p->s.ptr->s.size == nunits ){ 		// if their size sum == new size
				p->s.ptr=p->s.ptr->s.ptr;
				bp->s.size=nunits;
			}
			else {	 //現有的加p的下一塊>所求的大小					// if their size sum > new size
				(bp+nunits)->s.ptr=p->s.ptr->s.ptr;
				(bp+nunits)->s.size=bp->s.size+p->s.ptr->s.size-nunits;
				p->s.ptr=bp+nunits;
				bp->s.size=nunits;
			}
			freep=p;//避免p恰好指在p->s.ptr的位置
			return (void*)(bp+1);
		}
		else {	//寫完外產生一會記憶體的情況.與下一塊不相連				// if they are not continued or !> new size
			p=mymalloc(size);
			if (p==NULL)
				return NULL;
			for (i=0;i<bp->s.size-1;i++)
				*(p+i)=*((Header*)ptr+i);//複製資料
			myfree(ptr);
			return p;
		}
	}
	
}
//---------------------------------------------------------------------------------------------
void myfree ( void *ap){ //ap是要放link-list的memory的部份
	Header *bp, *p;
	
	if (ap==NULL)
		return;

	bp = (Header *)ap - 1;//指回去空間裡面的部份header的部份
	for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)//這裡是在找適合放回去link-list的位置
		if (p >= p->s.ptr && ( bp > p || bp < p->s.ptr))
			break;
	if (bp + bp->s.size == p->s.ptr)//整個link-list是按照address大小串起來的,所以那個迴圈在比較address來判斷要插哪.所以離開迴圈就是找到了
	{//這個條件句.因為連在一起.所以就把他合在一起
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	} else//沒連續就串起來
		bp->s.ptr = p->s.ptr;
	if (p + p->s.size == bp)
	{
		p->s.size += bp->s.size;
		p->s.ptr = bp->s.ptr;
	} else
		p->s.ptr = bp;
	freep = p;//增進效率用.morecore叫出的記憶體就直接指到
}
//---------------------------------------------------------------------------------------------
static Header *morecore(unsigned nu)//nu是指多少header的大小
{
	char *cp, *sbrk(int);  //sbrk is system function,不能直接,他是用來跟系統要空間的
	Header *up;

	if (nu<NALLOC)//避免一直跟系統去要空間,會沒效率,小於的話直接要nalloc這麼大的空間
		nu = NALLOC;
	cp = sbrk(nu * sizeof(Header));//就是再跟系統要空間,用cp去接
	sum=nu * sizeof(Header);
	if (cp== (char *) -1)//要不到空間的情況
		return NULL;
	up = (Header *) cp;
	up->s.size = nu;
	myfree((void *)(up+1));//這裡的myfree把空間放到linklist裡去,+1是跳過新增記憶體前面header所佔用的空間
	return freep;
}
