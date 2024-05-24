#include "PPMManager.h"
#include <stdbool.h>
#include <math.h>

#define HEIGHT 1000
#define WIDTH  1000
#define PAD 10

//number of points
#define N 20


#define POINT_COLOR BLACK

#define c1  0xFFAA0826
#define c2  0xFF88BB33
#define c3  0xFF3399C2
#define c4  0xFF892017
#define c5  0xFFFFFF00
#define c6  0xFFFF0000
#define c7  0xFF00FFFF
#define c8  0xFF2604A6
#define c9  0xFF218499
#define c10 0xFF90E49A
#define c11 YELLOW
#define c12 PINK
#define c13 BLUE
#define c14 0xFF902378
#define c15 0xFF102878
#define c16 0xFF238695
#define c17 0xFF86EEAA
#define c18 0xFFA7B4E8
#define c19 0xFF672BCC
#define c20 0xFFCC78AA

struct point{
    int x;
    int y;
};

uint32_t colors[N] = {c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15,c16,c17,c18,c19,c20};
struct point voronoi_points[N] = {0};

void CreateRandomCoordinates(int w, int h);
void PrintVoronoiPoints();
void CreatePoints(struct canvas c);
void FillVoronoi(struct canvas c);
bool ComparePixel(struct pixel p1, struct pixel p2);
float Distance(int x1, int y1, int x2, int y2);

int main(){
    srand(198);
    struct canvas c;
    CreateCanvas(&c, WIDTH, HEIGHT, MAX_COLOR);
    DrawBackground(&c, WHITE);
    CreateRandomCoordinates(WIDTH, HEIGHT);
    CreatePoints(c);
    FillVoronoi(c);
    //PrintVoronoiPoints();
    MakePPM("voronoi.ppm", &c);
    DestroyCanvas(&c);
    return 0;
}

void CreateRandomCoordinates(int w, int h){
    for (int i = 0; i < N; i++){

        int x = rand() % w;
        int y = rand() % h;

        voronoi_points[i].x = x;
        voronoi_points[i].y = y;
    }
}

void CreatePoints(struct canvas c){
    for(int i = 0; i < N; i++){
        DrawCrircle(&c, voronoi_points[i].x, voronoi_points[i].y,5,POINT_COLOR);
    }
}

void PrintVoronoiPoints(){
    for (int i = 0; i < N; i++){
        printf("Point %d:\n\tX: %d\n\tY: %d\n", i+1, voronoi_points[i].x, voronoi_points[i].y);
    }
}


void FillVoronoi(struct canvas c){
    int leastDistanceIndex = 0;
    float leastDistance = INT_MAX;
    float d;

    struct pixel circlePixel;
    ConvertColorToPixel(POINT_COLOR, &circlePixel);


    for(int i = 0; i < c.header.height*c.header.width; i++){

        if (ComparePixel(circlePixel, c.pixels[i])){
            continue;
        }

        else{
            for(int n = 0; n < N; n++){
                d = Distance(i%c.header.width,(int)i/c.header.height, voronoi_points[n].x, voronoi_points[n].y);
                if (d < leastDistance){
                    leastDistance = d;
                    leastDistanceIndex = n;
                }
            }
            
            leastDistance = INT_MAX;
            ConvertColorToPixel(colors[leastDistanceIndex], &c.pixels[i]);
        }
    }
}


bool ComparePixel(struct pixel p1, struct pixel p2){
    if (p1.r != p2.r) return false;
    if (p1.g != p2.g) return false;
    if (p1.b != p2.b) return false;
    if (p1.a != p2.a) return false;
    return true;
}



float Distance(int x1, int y1, int x2, int y2){
    float d;
    int dx = x1 - x2;
    int dy = y1 - y2;
    d = sqrtf((dx*dx) + (dy*dy));
    return d;
}
