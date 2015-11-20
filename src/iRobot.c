#include <stdio.h>
#include <errno.h>
#include <string.h>

int pic_num = 0;

int main()
{
    takePicture();
    return 0;
}

void takePicture()
{
    char out[BUFSIZ];
    sprintf(out,"fswebcam -r 1280x720 --no-banner ./pictures/image%i.jpg",pic_num);
    system(out);
    pic_num++;
}
