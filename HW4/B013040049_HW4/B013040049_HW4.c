#include <string.h>
#include <stdio.h>
#include "mm.h"

int main(void){
    int size = 0;
    int i;
    int choice;
    printf("1.malloc_test\n"
           "2.calloc_test\n"
           "3.realloc_test\n"
           "4.exit\n: ");
    scanf("%d", &choice);

    while(choice!=4){
        if(choice==1){
                int *ptr = mymalloc(sizeof(int));

                printf("空間位置:%p\n", ptr);
                printf("空間儲存值：%d\n", *ptr);

                *ptr = 200;

                printf("空間位置:%p\n", ptr);
                printf("空間儲存值：%d\n", *ptr);

                myfree(ptr);
        }

        if(choice==2){
                printf("請輸入陣列長度：");
                scanf("%d", &size);
                int *arr = mycalloc(size,sizeof(int));

                printf("顯示元素值：\n");
                for(i = 0; i < size; i++) {
                    printf("arr[%d] = %d\n", i, *(arr+i));
                }

                printf("指定元素值：\n");
                for(i = 0; i < size; i++) {
                    printf("arr[%d] = ", i);
                    scanf("%d" , arr + i);
                }

                printf("顯示元素值：\n");
                for(i = 0; i < size; i++) {
                    printf("arr[%d] = %d\n", i, *(arr+i));
                }
                myfree(arr);
        }

        if(choice==3){
                printf("請輸入陣列長度：");
                scanf("%d", &size);
                int *arr1 = mymalloc(size * sizeof(int));

                printf("指定元素值：\n");
                for(i = 0; i < size; i++) {
                    printf("arr1[%d] = ", i);
                    scanf("%d" , arr1 + i);
                }

                printf("顯示元素值：\n");
                for(i = 0; i < size; i++) {
                    printf("arr1[%d] = %d\n", i, *(arr1+i));
                }

                int *arr2 = myrealloc(arr1, sizeof(int) * size * 2);
                printf("顯示元素值：\n");
                for(i = 0; i < size * 2; i++) {
                    printf("arr2[%d] = %d\n", i, *(arr2+i));
                }

                printf("arr1 address: %p\n", arr1);
                printf("arr2 address: %p\n", arr2);

                myfree(arr2);
        }

        if(choice==4){
            myfree(NULL);
        }

        printf("\n\n1.malloc_test\n"
               "2.calloc_test\n"
               "3.realloc_test\n"
               "4.exit\n: ");
        scanf("%d", &choice);
    }
    return 0;
}

