#include <stdbool.h>
#include <string.h>
#include "PPMManager.h"
#include <math.h>

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

void CreateGrayscaleImage();
void CreateCheckeredImage();
void CreateCircleImage();
void TestImages();
void TestImage(char* TestImagePath, char* CorrectImagePath);

bool CompareCanvas(canvas* TestCanvas, canvas* CorrectCanvas);
bool ComparePixel(pixel p1, pixel p2);

//Voronoi Functions
void CreateRandomCoordinates(int w, int h);
void PrintVoronoiPoints();
void CreatePoints(canvas c);
void FillVoronoi(canvas c);
float Distance(int x1, int y1, int x2, int y2);

int main(){
    TestImages();
    return 0;
}


void CreateVoronoiImage(){
    int width = 1000;
    int height = 1000;
    srand(198);
    canvas c = CreateCanvas(width, height, MAX_COLOR);
    DrawBackground(&c, WHITE);
    CreateRandomCoordinates(width, height);
    FillVoronoi(c);
    CreatePoints(c);
    MakePPM("./TestImages/voronoi.ppm", &c);
    DestroyCanvas(&c);
}

void CreateGrayscaleImage(){
    canvas c;
    ReadPPM("./TestImages/Thesis_img1.ppm", &c);
    GrayScaleImage(&c);
    MakePPM("./TestImages/grayscale.ppm", &c);
    DestroyCanvas(&c);
}


void CreateCheckeredImage(){
    int rows = 10;
    int cols = 10;
    int height = 300;
    int width = 300;
    int cellWidth = width/cols;
    int cellHeight = height/rows;

    canvas c = CreateCanvas(width,height,MAX_COLOR);
    DrawBackground(&c, LIGHT_BLUE);
    for (int i = 0; i < cols; i++){
        for (int j = 0; j < rows; j++){
            if ((i+j) % 2 == 0){
                DrawRectangle(&c,j*cellWidth, i*cellHeight,cellWidth,cellHeight,RED);
            }
        }
    }
    MakePPM("./TestImages/checkered.ppm", &c);
    DestroyCanvas(&c);

}

void CreateCircleImage(){
    int height = 200;
    int width = 400;
    pixel pixels[height*width];
    
    canvas c = CreateCanvasWithPixels(width,height,MAX_COLOR,pixels);
    DrawBackground(&c, WHITE);
    DrawCrircle(&c, width/2, height/2, height/4, RED);
    MakePPM("./TestImages/circle.ppm", &c);
}

void TestImages(){
    CreateCheckeredImage();
    CreateCircleImage();
    CreateGrayscaleImage();
    CreateVoronoiImage();
    TestImage("./TestImages/checkered.ppm","./TestImages/checkered_correct.ppm");
    TestImage("./TestImages/circle.ppm","./TestImages/circle_correct.ppm");
    TestImage("./TestImages/grayscale.ppm","./TestImages/grayscale_correct.ppm");
    TestImage("./TestImages/voronoi.ppm","./TestImages/voronoi-correct.ppm");
}
void TestImage(char* TestImagePath, char* CorrectImagePath){
    canvas TestCanvas;
    canvas CorrectCanvas;
    ReadPPM(TestImagePath, &TestCanvas);
    ReadPPM(CorrectImagePath, &CorrectCanvas);
    if(CompareCanvas(&TestCanvas, &CorrectCanvas)){
        printf("\nGOOD: %s and %s are the same", TestImagePath, CorrectImagePath);
    }
    else{
        printf("\nERROR: %s and %s are NOT the same", TestImagePath, CorrectImagePath);
    }
    DestroyCanvas(&TestCanvas);
    DestroyCanvas(&CorrectCanvas);
}

bool CompareCanvas(canvas* TestCanvas, canvas* CorrectCanvas){
    if (strcmp(TestCanvas->header.magicNum,CorrectCanvas->header.magicNum)) return false;
    if (TestCanvas->header.maxColor != CorrectCanvas->header.maxColor) return false;
    if (TestCanvas->header.height != CorrectCanvas->header.height) return false;
    if (TestCanvas->header.width != CorrectCanvas->header.width) return false;
    for (int i = 0; i < TestCanvas->header.height*TestCanvas->header.width; i++){
        if (!ComparePixel(TestCanvas->pixels[i], CorrectCanvas->pixels[i])) return false;
    }
    return true;
}

bool ComparePixel(pixel p1, pixel p2){
    if (p1.r != p2.r) return false;
    if (p1.g != p2.g) return false;
    if (p1.b != p2.b) return false;
    if (p1.a != p2.a) return false;
    return true;
}


void CreateRandomCoordinates(int w, int h){
    for (int i = 0; i < N; i++){

        int x = rand() % w;
        int y = rand() % h;

        voronoi_points[i].x = x;
        voronoi_points[i].y = y;
    }
}

void CreatePoints(canvas c){
    for(int i = 0; i < N; i++){
        DrawCrircle(&c, voronoi_points[i].x, voronoi_points[i].y,5,POINT_COLOR);
    }
}

void FillVoronoi(canvas c){
    int leastDistanceIndex = 0;
    float leastDistance = INT_MAX;
    float d;

    pixel circlePixel;
    ConvertColorToPixel(POINT_COLOR, &circlePixel);


    for(int i = 0; i < c.header.height*c.header.width; i++){
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

float Distance(int x1, int y1, int x2, int y2){
    float d;
    int dx = x1 - x2;
    int dy = y1 - y2;
    d = sqrtf((dx*dx) + (dy*dy));
    return d;
}
