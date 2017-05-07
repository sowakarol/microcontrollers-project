#include "string.h"
#include <stdio.h>


#define V_BRIGHT 2000
#define BRIGHT 1000
#define DIM 500
#define V_DIM 200


typedef enum {V_DIM, DIM, V_BRIGHT, BRIGHT} brightness_t;
int main(void)
{

    int d = brightness_t[1];

    printf("%d", d);

    return 0;
}