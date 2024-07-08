#include <stdio.h>
#include <stdint.h>

uint8_t div(uint8_t a, uint8_t b)
{
    if (b == 0) {
        return 0xff;
    }
}

int main()
{
    uint8_t ret = div(12, 2);
    printf("%d\n", ret);
    return 0;
}
/*
(base) ykim@ykim-SBT:~/devel/cpp/cpp_examples/c/compiler_option$ gcc -Wall -Wreturn-type option.c 
option.c: In function ‘div’:
option.c:9:1: warning: control reaches end of non-void function [-Wreturn-type]
    9 | }
      | ^
(base) ykim@ykim-SBT:~/devel/cpp/cpp_examples/c/compiler_option$ gcc -Wreturn-type option.c 
option.c: In function ‘div’:
option.c:9:1: warning: control reaches end of non-void function [-Wreturn-type]
    9 | }
      | ^
(base) ykim@ykim-SBT:~/devel/cpp/cpp_examples/c/compiler_option$ gcc  option.c 
(base) ykim@ykim-SBT:~/devel/cpp/cpp_examples/c/compiler_option$  
 */