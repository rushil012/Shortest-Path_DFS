#include <stdio.h>

// #define TRACE_D(E) printf(#E " = %d\n", E)
// #define TRACE_S(E) printf(#E " = %s\n", E)

#include <ctype.h>
int rt(char ch) {
    return 4 * isdigit(ch) != 0 + 2 * islower(ch) != 0 + isalnum(ch) != 0; 
}

int main() {
    int a = rt('A');
    int b = rt('a');
    int c = rt(' ');
    printf("%d\n%d\n%d",a,b,c);
}