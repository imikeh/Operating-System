#ifndef MMANAGE_H_
#define MMANAGE_H_

#include <stddef.h>

union header {
	struct {
		union header *ptr;
		unsigned size;
	}s;
	long x; //當作使用的記憶體,使用long是因為可以撐大可以撐大headerr,因為因為x.s共用一個記憶體,所以是以大的為主
};
typedef union header Header;

void *mymalloc(size_t size); /*用來產生動態的記憶體空間,size為記憶體空間的大小,若成功則回傳指向該空間的指標*/

void myfree(void *ptr); /*釋放原先所建立指向的記憶體空間,並非刪除原先記憶體空間的資料,也非將操作的指標變數改為指向NULL,而是告訴OS說,程式不會再去利用這塊記憶體空間*/

void *mycalloc(size_t nmemb, size_t size); /*用來產生產生動態陣列的記憶體空間,第一個參數為陣列的元素個數,第二個參數為元素所佔記憶體空間的大小,成功則回傳指向該空間的指標*/

void *myrealloc(void *ptr, size_t size); /*可改變由malloc(),calloc()或realloc()所配置記憶體空間的大小,第一個參數為指向原始空間的指標,第二個參數為新空間的大小,
                                            成功則回傳指向該重新分配空間的指標*/
#endif // end of #ifndef MMANAGE_H_
