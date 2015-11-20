#include <stdio.h>
#include <errno.h>
#include <string.h>

int main()
{
    char out[BUFSIZ];
    strcpy(out, system("ls"));
    printf("%s\n",out);
    return 0;
}

