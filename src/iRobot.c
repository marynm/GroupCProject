#include <stdio.h>
#include <errno.h>
#include <string.h>

void takePicture();
int pic_num = 0;
char out[BUFSIZ];

int main()
{
    takePicture();
    return 0;
}

void takePicture()
{
    //create a linux command string to take a picture using a webcam and properly date them
    sprintf(out,"fswebcam -r 1280x720 --no-banner ./pictures/image%i.jpg",pic_num);
    //system will execute linux command created above
    system(out);
    pic_num++;
}
