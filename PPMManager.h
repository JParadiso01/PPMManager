#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PPMManager
#define PPMManager

#define MAX_COLOR   255
#define RED         0xFFFF0000
#define GREEN       0xFF00FF00
#define BLUE        0xFF0000FF
#define YELLOW      0xFFFFFF00
#define PINK        0xFFFF00FF
#define WHITE       0xFFFFFFFF
#define LIGHT_BLUE  0xFF00FFFF
#define BLACK       0xFF000000
#define GRAY        0xFF202020

typedef struct {
    char magicNum[3];
    int width;
    int height;
    int maxColor;
}PPMHeader;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} pixel;

typedef struct {
    PPMHeader header;
    pixel* pixels;
} canvas;

//draw primitive 2d shapes into an array of pixels
void DrawCrircle(canvas* canvas, int x, int y, int r, uint32_t color);
void DrawRectangle(canvas* canvas, int x, int y, int l, int h, uint32_t color);
void DrawTriangle(canvas* canvas, int x, int y, int l, uint32_t color);
void DrawHorizontalLine(canvas* canvas, int row, uint32_t color);
void DrawVerticalLine(canvas* canvas, int row, uint32_t color);
void DrawLine(canvas* canvas, int x1, int y1, int x2, int y2, uint32_t color);
void DrawBackground(canvas* canvas, uint32_t color);

//add filters to images
void GrayScaleImage(canvas* canvas);

//create datatypes for using header file
canvas CreateCanvas(const int width, const int height, const int maxColor);
canvas CreateCanvasWithPixels(const int width, const int height, const int maxColor, pixel* pixels);
void DestroyCanvas(canvas* canvas);  
void MakePPM(char* OutputFileName, canvas* canvas);
void WritePPMHeader(FILE* file, PPMHeader* header);
void ReadPPM(const char* FilePath, canvas* canvas);
void ConvertColorToPixel(uint32_t color, pixel* pixel);

//print information
void PrintPixel(const pixel pixel);
void PrintPixels(const canvas* canvas);
void PrintCanvas(const canvas* canvas);
void PrintPPMHeader(const PPMHeader header);



void DrawBackground(canvas* canvas, uint32_t color){
    pixel p;
    ConvertColorToPixel(color, &p);
    for (int i = 0; i < canvas->header.height*canvas->header.width; i++){
        canvas->pixels[i] = p;
    }
}

void MakePPM(char* OutputFileName, canvas* canvas){
    FILE* output = fopen(OutputFileName, "wb");
    if (output == NULL){
        perror("\nERROR: Unable to create file");
    }

    WritePPMHeader(output, &canvas->header);

    //input pixels into image
    for (int i = 0; i < canvas->header.width*canvas->header.height; i++){
        fwrite((char *)&canvas->pixels[i].r, sizeof(uint8_t), 1, output);
        fwrite((char *)&canvas->pixels[i].g, sizeof(uint8_t), 1, output);
        fwrite((char *)&canvas->pixels[i].b, sizeof(uint8_t), 1, output);
    }
    fclose(output);
    printf("\nINFO: %s has been created", OutputFileName);
}

void WritePPMHeader(FILE* file, PPMHeader* header)
{
    fprintf(file, "%s\n", header->magicNum);
    fprintf(file, "%d %d\n", header->width, header->height);
    fprintf(file, "%d\n", header->maxColor);
}

void ConvertColorToPixel(uint32_t hex, pixel* p){
    p->a = hex >> 24 & 0xFF;
    p->r = hex >> 16 & 0xFF;
    p->g = hex >> 8  & 0xFF;
    p->b = hex >> 0  & 0xFF;
}
void PrintPixel(const pixel p){
    printf("pixel r: %x ", p.r);
    printf("pixel g: %x ", p.g);
    printf("pixel b: %x, ", p.b);
}
void PrintPixels(const canvas* canvas){
    pixel p;
    for (int i = 0; i < canvas->header.height*canvas->header.width; i++){
        p = canvas->pixels[i];
        printf("pixel %d r: %x ", i, p.r);
        printf("pixel %d g: %x ", i, p.g);
        printf("pixel %d b: %x, ", i, p.b);
    }
}

void DrawHorizontalLine(canvas* canvas, int row, uint32_t color){
    pixel p;
    ConvertColorToPixel(color, &p);
    for (int i = 0; i < canvas->header.width; i++){
        canvas->pixels[(canvas->header.height*row)+i] = p;
    }
}

void DrawLine(canvas* canvas, int x1, int y1, int x2, int y2, uint32_t color){
    //y = mx + b
    int temp;
    int dy = y2 - y1;
    int dx = x2 - x1;
    int b = y1/(((dy/dx)*x1)+1);
    pixel p;
    
    ConvertColorToPixel(color, &p);
    if (x1 > x2){
        temp = x2;
        x2 = x1;
        x1 = temp;
    }
    if (y1 > y2){
        temp = y2;
        y2 = y1;
        y1 = temp;
    }
    for (int x = x1; x < x2; x++){
        for (int y = y1; y < y2; y++){
            if (y == (((dy/dx)*x)+b)){
                canvas->pixels[(canvas->header.height*y)+x] = p;
            }
        }
    }
    
}

void DrawVerticalLine(canvas* canvas, int col, uint32_t color){
    pixel p;
    ConvertColorToPixel(color, &p);
    for (int i = 0; i <= canvas->header.height; i++){
        canvas->pixels[(canvas->header.width*i)+col] = p;
    }
}
void DrawRectangle(canvas* canvas, int x, int y, int l, int h, uint32_t color){
    pixel p;
    ConvertColorToPixel(color, &p);
    for (int i = x; i < x+l; i++){
        for (int j = y; j < y+h; j++){
            if (((canvas->header.width*j)+i) < 0 || ((canvas->header.width*j)+i) >= canvas->header.height*canvas->header.width) {
                continue;
            }
            else {
                canvas->pixels[(canvas->header.width*j)+i] = p;
            }
        }
    }
}

//Makes a right triangle
void DrawTriangle(canvas* canvas, int x, int y, int l, uint32_t color){
    pixel p;
    ConvertColorToPixel(color, &p);
    int len = l;
    for (int j = y; j <= y+len; j++){
        for (int i = x; i <= x+l; i++){
            if (((canvas->header.width*j)+i) < 0 || ((canvas->header.width*j)+i) >= canvas->header.height*canvas->header.width) {
                continue;
            }
            else {
                canvas->pixels[(canvas->header.height*j)+i] = p;
            }
        }
        l--;
    }
}
void GrayScaleImage(canvas* canvas){
    for (int i = 0; i < canvas->header.height*canvas->header.width; i++){
        uint32_t avg = ((canvas->pixels[i].r)*0.21 + (canvas->pixels[i].g)*0.72 + (canvas->pixels[i].b)*0.07);
        canvas->pixels[i].r = avg;
        canvas->pixels[i].g = avg;
        canvas->pixels[i].b = avg;
    }
}

void DrawCrircle(canvas* canvas, int x, int y, int r, uint32_t color){
    pixel p;
    ConvertColorToPixel(color, &p);
    int x1 = x - r;
    int x2 = x + r;
    int y1 = y - r;
    int y2 = y + r;

    for (int i = x1; i <= x2; i++){
        for (int j = y1; j <= y2; j++){
            int dx = x - i;
            int dy = y - j;
            if ((dx*dx) + (dy*dy) < (r*r)) {
                if (((canvas->header.width*j)+i) < 0 || ((canvas->header.width*j)+i) >= canvas->header.height*canvas->header.width) {
                    continue;
                }
                else {
                    canvas->pixels[(j*canvas->header.width)+i] = p;
                }
            }
        }
    }
}
void ReadPPM(const char* FilePath, canvas* canvas){
    FILE* input = fopen(FilePath, "rb");
    if (input == NULL){
        printf("\nERROR: Unable to read %s", FilePath);
        exit(1);
    }
    //get header
    fscanf(input, "%s %d %d %d",canvas->header.magicNum,&canvas->header.width,&canvas->header.height,&canvas->header.maxColor);
    //get pixels
    canvas->pixels = malloc(sizeof(uint32_t)*canvas->header.height*canvas->header.width);
    pixel p;
    for (int i = 0; i < canvas->header.height*canvas->header.width; i++){ 
        fread(&p.r, sizeof(uint8_t), 1, input);
        fread(&p.g, sizeof(uint8_t), 1, input);
        fread(&p.b, sizeof(uint8_t), 1, input);
        canvas->pixels[i] = p;
    }
    fclose(input);

    //PrintCanvas(canvas);
    printf("\nINFO: %s has been read", FilePath);
}

void PrintCanvas(const canvas* canvas){
    pixel p;
    printf("\nCanvas:");
    printf("\nWidth: %d", canvas->header.width);
    printf("\nheight: %d", canvas->header.height);
    printf("\nPixels:\n");
    for (int i = 0; i < canvas->header.height*canvas->header.width; i++){
        p = canvas->pixels[i];
        printf("%x ",p.r);
        printf("%x ",p.g);
        printf("%x ",p.b);
    }
}

void PrintPPMHeader(const PPMHeader header){
    printf("\nPPMHeader:\nMagic Num: %s\nWidth: %d\nHeight: %d\nMax Color: %d\n", header.magicNum, header.width, header.height, header.maxColor);
}

canvas CreateCanvas(const int width, const int height, const int maxColor){

    canvas* canvas = malloc(sizeof(canvas));
    canvas->header.magicNum[0] = 'P';
    canvas->header.magicNum[1] = '6';
    canvas->header.magicNum[2] = '\0';
    canvas->header.width = width;
    canvas->header.height = height;
    canvas->header.maxColor = maxColor;
    canvas->pixels = malloc(sizeof(pixel)*canvas->header.width*canvas->header.height);
    return *canvas;
}

canvas CreateCanvasWithPixels(const int width, const int height, const int maxColor, pixel* pixels){
    canvas* canvas = malloc(sizeof(canvas));
    canvas->header.magicNum[0] = 'P';
    canvas->header.magicNum[1] = '6';
    canvas->header.magicNum[2] = '\0';
    canvas->header.width = width;
    canvas->header.height = height;
    canvas->header.maxColor = maxColor;
    canvas->pixels = malloc(sizeof(pixel)*canvas->header.width*canvas->header.height);
    canvas->pixels = pixels;
    return *canvas;
}

void DestroyCanvas(canvas* canvas){
    free(canvas->pixels);
}

#endif


// TODOS:
// Triangles with 3 points
// More filters
// Rotate Image
// 