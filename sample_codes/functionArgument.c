#include <stdio.h>

void print()  
{
    printf("Hello World!");
}

void helloworld(void (*f)())  
{
    f();
}

int main(void)  
{
    helloworld(print);
    return (0);
}
