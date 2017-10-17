#include <stdio.h>
#include "mm.c"

int main(void)
{
    int *a=NULL, i=0, choice, num;

    printf("1.malloc_test\n"
           "2.calloc_test\n"
           "3.realloc_test\n"
           "4.exit\n: ");
    scanf("%d", &choice);

    while(choice!=4)
    {
        switch(choice)
        {
            case 1:
                printf("array_size: ");
                scanf("%d", &num);
                a=(int*)mymalloc(sizeof(int)*num);
                int reset;
                printf("reset element value?(1.yes 2.no): ");
                scanf("%d", &reset);
                if(reset==1)
                {
                    for(i=0; i<num; i++)
                    {
                        printf("a[%d]: ", i);
                        scanf("%d", &a[i]);
                    }
                }

                printf("array_address: %p\n", a);
                printf("array_element: ");
                for(i=0; i<num; i++)
                {
                    printf("%d ", a[i]);
                }
                break;

            case 2:
                printf("array_size: ");
                scanf("%d", &num);
                a=(int*)mycalloc(sizeof(int), num);
                printf("array_address: %p\n", a);
                printf("array_element: ");
                for(i=0; i<num; i++)
                {
                    printf("%d ", a[i]);
                }
                break;

            case 3:
                if(a==NULL)
                {
                    printf("a=NULL");
                    break;
                }
                printf("array_size: ");
                scanf("%d", &num);
                int *b=myrealloc(a, sizeof(int)*num);
                printf("array_address: %p\n", b);
                printf("array_element: ");
                for(i=0; i<num; i++)
                {
                    printf("%d ", b[i]);
                }
                break;
        }

        printf("\n\n1.malloc_test\n"
               "2.calloc_test\n"
               "3.realloc_test\n"
               "4.exit\n: ");
        scanf("%d", &choice);
    }

    return 0;
}
