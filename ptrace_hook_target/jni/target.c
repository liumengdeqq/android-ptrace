#include <stdio.h>
         
int flag = 1;
int count = 0;
         
int sub()
{
    printf("Sub call.\n");
    return 1;
}
         
int main()
{   
    while(flag)
    {
            printf("Sub return:%d\n", sub());
            count++;
            sleep(3);
        }
    return 0;
}
