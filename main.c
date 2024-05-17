#include "PPMManager.h"

#define HEIGHT        300
#define WIDTH         300

int main(){
    struct canvas c;
    CreateCanvas(&c, WIDTH, HEIGHT, MAX_COLOR);
    DrawBackground(&c, BLUE);
    DrawTriangle(&c, WIDTH/3,HEIGHT/3,100, YELLOW);
    DrawCrircle(&c, HEIGHT/2, WIDTH/2, 50, GREEN);
    DrawLine(&c, 0,0, WIDTH, HEIGHT, RED);
    DrawLine(&c, 0,HEIGHT,WIDTH, 0, RED);
    DrawLine(&c, 13,5, 19,50,PINK);
    MakePPM("image.ppm", &c);
    DestroyCanvas(&c);
    return 0;
}