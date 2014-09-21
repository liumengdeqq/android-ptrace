#include <stdio.h>


int flag = 1;
int count = 0;

int main() {
    char *str = "abcdefg";
    while(flag){
        printf("Target is running: %d\n", count);
        count++;
        sleep(3);
    }
    return 0;
}
