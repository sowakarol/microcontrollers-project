// #include "string.h"
// #include <stdio.h>

// int main(void)
// {
//     char *txt = "12345";
//     char substr[5] = "alad";
//     char spaces[4] = "   ";
//     int EMPTY = 3;
//     int nchars = 1;
//     int go = 1;
//     while (go)
//     {
//         if (EMPTY >= 0)
//         {
//             strncpy(substr, spaces, EMPTY);
//             strncpy(substr + EMPTY--, txt, nchars++);
//             printf("|%c|%c|%c|%c|\n", substr[0],substr[1],substr[2],substr[3]);
//             // EMPTY--;
//             go = 1;
//         } else {
//             go = 0;
//         }
//     }

//     // printf("%s\n", substr);
//     // strncpy(substr, spaces, 3);
//     // printf("%s\n", substr);
//     // strncpy(substr+3, txt, 1);
//     // printf("%s\n", substr);
//     // strncpy(substr, spaces, 2);
//     // printf("%s\n", substr);
//     // strncpy(substr+2, txt, 2);
//     // printf("%s\n", substr);
    
//     // strncpy(substr, spaces, 4);
//     // printf("%s\n", substr);
//     // strncpy(substr, spaces, 4);
//     // printf("%s\n", substr);
//     // strncpy(substr, spaces, 4);
//     // printf("%s\n", substr);

//     return 0;
// }