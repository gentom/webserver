#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int main(void){
    char buff[100];
    scanf("%s",buff);
    strcat(buff, "test");
    printf("result: %s\n", buff);
    return 0;
}