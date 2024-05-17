#include <stdbool.h>
#include <string.h>
#include "PPMManager.h"

#define HEIGHT        300
#define WIDTH         300

void CreateGrayscaleImage();
void CreateCheckeredImage();
void CreateCircleImage();
void TestImages();
void TestImage(char* TestImagePath, char* CorrectImagePath);
bool CompareCanvas(struct canvas* TestCanvas, struct canvas* CorrectCanvas);
bool ComparePixel(struct pixel p1, struct pixel p2);

int main(){
    TestImages();
    return 0;
}


void CreateGrayscaleImage(){
    struct canvas c;
    ReadPPM("./TestImages/Thesis_img1.ppm", &c);
    GrayScaleImage(&c);
    MakePPM("./TestImages/grayscale.ppm", &c);
    DestroyCanvas(&c);
}


void CreateCheckeredImage(){
    struct canvas c;
    int rows = 10;
    int cols = 10;
    int height = 300;
    int width = 300;
    int cellWidth = width/cols;
    int cellHeight = height/rows;

    CreateCanvas(&c,width,height,MAX_COLOR);
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
    struct canvas c;
    int height = 200;
    int width = 400;
    struct pixel pixels[height*width];
    CreateCanvasWithPixels(&c,width,height,MAX_COLOR,pixels);
    DrawBackground(&c, WHITE);
    DrawCrircle(&c, width/2, height/2, height/4, RED);
    MakePPM("./TestImages/circle.ppm", &c);
}

void TestImages(){
    CreateCheckeredImage();
    CreateCircleImage();
    CreateGrayscaleImage();
    TestImage("./TestImages/checkered.ppm","./TestImages/checkered_correct.ppm");
    TestImage("./TestImages/circle.ppm","./TestImages/circle_correct.ppm");
    TestImage("./TestImages/grayscale.ppm","./TestImages/grayscale_correct.ppm");
}
void TestImage(char* TestImagePath, char* CorrectImagePath){
    struct canvas TestCanvas;
    struct canvas CorrectCanvas;
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

bool CompareCanvas(struct canvas* TestCanvas, struct canvas* CorrectCanvas){
    if (strcmp(TestCanvas->header.magicNum,CorrectCanvas->header.magicNum)) return false;
    if (TestCanvas->header.maxColor != CorrectCanvas->header.maxColor) return false;
    if (TestCanvas->header.height != CorrectCanvas->header.height) return false;
    if (TestCanvas->header.width != CorrectCanvas->header.width) return false;
    for (int i = 0; i < TestCanvas->header.height*TestCanvas->header.width; i++){
        if (!ComparePixel(TestCanvas->pixels[i], CorrectCanvas->pixels[i])) return false;
    }
    return true;
}

bool ComparePixel(struct pixel p1, struct pixel p2){
    if (p1.r != p2.r) return false;
    if (p1.g != p2.g) return false;
    if (p1.b != p2.b) return false;
    if (p1.a != p2.a) return false;
    return true;
}