//Question 2 code at the bottom.
//This is copied from the project 2 outline.

#include "lab7q2_img.h"
#include <stdio.h>
#include <math.h>

void create_img(struct rgb_img **im, size_t height, size_t width){
    *im = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*im)->height = height;
    (*im)->width = width;
    (*im)->raster = (uint8_t *)malloc(3 * height * width);
}


int read_2bytes(FILE *fp){
    uint8_t bytes[2];
    fread(bytes, sizeof(uint8_t), 1, fp);
    fread(bytes+1, sizeof(uint8_t), 1, fp);
    return (  ((int)bytes[0]) << 8)  + (int)bytes[1];
}

void write_2bytes(FILE *fp, int num){
    uint8_t bytes[2];
    bytes[0] = (uint8_t)((num & 0XFFFF) >> 8);
    bytes[1] = (uint8_t)(num & 0XFF);
    fwrite(bytes, 1, 1, fp);
    fwrite(bytes+1, 1, 1, fp);
}

void read_in_img(struct rgb_img **im, char *filename){
    FILE *fp = fopen(filename, "rb");
    size_t height = read_2bytes(fp);
    size_t width = read_2bytes(fp);
    create_img(im, height, width);
    fread((*im)->raster, 1, 3*width*height, fp);
    fclose(fp);
}

void write_img(struct rgb_img *im, char *filename){
    FILE *fp = fopen(filename, "wb");
    write_2bytes(fp, im->height);
    write_2bytes(fp, im->width);
    fwrite(im->raster, 1, im->height * im->width * 3, fp);
    fclose(fp);
}

uint8_t get_pixel(struct rgb_img *im, int y, int x, int col){
    return im->raster[3 * (y*(im->width) + x) + col];
}

void set_pixel(struct rgb_img *im, int y, int x, int r, int g, int b){
    im->raster[3 * (y*(im->width) + x) + 0] = r;
    im->raster[3 * (y*(im->width) + x) + 1] = g;
    im->raster[3 * (y*(im->width) + x) + 2] = b;
}

void destroy_image(struct rgb_img *im)
{
    free(im->raster);
    free(im);
}


void print_grad(struct rgb_img *grad){
    int height = grad->height;
    int width = grad->width;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%d\t", get_pixel(grad, i, j, 0));
        }
    printf("\n");    
    }
}

// new function for Question 2 of the lab
void change_brightness(struct rgb_img *im, double brightness){
    //get dimension from struct
    int height = im->height;
    int width = im->width;

    //loop through to get each pixel
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            //get pixel values from 0, 1, 2 as r, g, b
            int r = get_pixel(im, i, j, 0);
            int g = get_pixel(im, i, j, 1);
            int b = get_pixel(im, i, j, 2);

            //used double because values 0<x<1 in int will just become 0
            r = r * brightness;
            g = g * brightness;
            b = b * brightness;
           
           //make sure values are within 0-255
            if(r > 255){
                r = 255;
            }
            if(g > 255){
                g = 255;
            }
            if(b > 255){
                b = 255;
            }
            if(r < 0){
                r = 0;
            }
            if(g < 0){
                g = 0;
            }
            if(b < 0){
                b = 0;
            }
            //store the new values of rgb back into the pixel at i, j in struct im
            set_pixel(im, i, j, r, g, b);
        }
    }
} 

void main(){
    struct rgb_img *im;
    read_in_img(&im, "input.bin");
    double brightness = 1;
    change_brightness(im, brightness);
    write_img(im, "cannon1x.bin");
}