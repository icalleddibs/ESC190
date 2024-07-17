#include "c_img.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void calc_energy(struct rgb_img *im, struct rgb_img ** grad){
    int height = im->height;
    int width = im->width;
    int rx, gx, bx, ry, gy, by, deltax, deltay;
    create_img(grad, im->height, im->width);

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){

            //X VALUES (columns)
            if (x == 0){
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, width-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, width-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, width-1, 2);
            } else if (x == width-1){
                rx = get_pixel(im, y, 0, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, 0, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, 0, 2) - get_pixel(im, y, x-1, 2);
            } else {
                rx = get_pixel(im, y, x+1, 0) - get_pixel(im, y, x-1, 0);
                gx = get_pixel(im, y, x+1, 1) - get_pixel(im, y, x-1, 1);
                bx = get_pixel(im, y, x+1, 2) - get_pixel(im, y, x-1, 2);
            }
            //Y VALUES (rows)
            if (y == 0){
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, height-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, height-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, height-1, x, 2);
            } else if (y == height-1){
                ry = get_pixel(im, 0, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, 0, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, 0, x, 2) - get_pixel(im, y-1, x, 2);
            } else {
                ry = get_pixel(im, y+1, x, 0) - get_pixel(im, y-1, x, 0);
                gy = get_pixel(im, y+1, x, 1) - get_pixel(im, y-1, x, 1);
                by = get_pixel(im, y+1, x, 2) - get_pixel(im, y-1, x, 2);
            }

            deltay = pow(ry, 2) + pow(gy, 2) + pow(by, 2);
            deltax = pow(rx, 2) + pow(gx, 2) + pow(bx, 2);
            uint8_t energy = sqrt(deltax + deltay) / 10;

            set_pixel(*grad, y, x, energy, energy, energy); 
        }
    }

}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    int height = grad->height;
    int width = grad->width;

    //allocate memory for best arr
    *best_arr = (double *)malloc((height * width +1) * sizeof(double));
    //added plus 1 BECAUSE i set best[height*width] to 0 so i need to allocate that space
    //this is what was causing my issues, it couldn't access this index because it wasn't malloced!
    //likely not the best approach but it works with the test code. issues will likely be here.

    //first row: just keep top pixels, any colour (same energy)
    for (int i = 0; i < width; i++){
        (*best_arr)[i] = get_pixel(grad, 0, i, 0);
    }
    
    double cl, ca, cr, cost; //costs above, left, right
    int cur_energy;
    for (int y = 1; y < height; y++){
        for (int x = 0; x < width; x++){
            cur_energy = get_pixel(grad, y, x, 0); //energy of current pixel
            if (x == 0){
                ca = (double) (*best_arr)[(y-1)*width+x];
                cr = (double) (*best_arr)[(y-1)*width+x+1];
                cost = fmin(ca, cr);
                (*best_arr)[y*width+x] = cost + cur_energy;
            } else if (x == width-1){
                cl = (double) (*best_arr)[(y-1)*width+x-1];
                ca = (double) (*best_arr)[(y-1)*width+x];
                cost = fmin(cl, ca);
                (*best_arr)[y*width+x] = cost + cur_energy;
            } else {
                cl = (double) (*best_arr)[(y-1)*width+x-1];
                ca = (double) (*best_arr)[(y-1)*width+x];
                cr = (double) (*best_arr)[(y-1)*width+x+1];
                cost = fmin(cl, fmin(ca, cr));
                (*best_arr)[y*width+x] = cost + cur_energy;
            }
        }
    }
    (*best_arr)[height*width] = 0; //last pixel is 0
}

void recover_path_rec(double *best, int height, int width, int **path){
    if (height == 0){ //base case, no more rows above it
        return; 

    //if this is the last row, then find the min of the last row and store the index
    } else if (best[height*width] == 0){ 
        int cur_min, cur_min_index;
        cur_min = best[(height-1)*width]; //first of row
        cur_min_index = 0; //first of row
        for (int i = 1; i < width; i++){
            cur_min = fmin(cur_min, best[(height-1)*width+i]);
            if (cur_min == best[(height-1)*width+i]){ 
                cur_min_index = i;
                }
            }
        (*path)[height-1] = cur_min_index;
        recover_path_rec(best, height-1, width, path);

    //this is for all the other rows, just look at the one above
    //if there are ties, it will take the right-most pixel.

    } else {
        //prev_index holds the "pivot" index from the previous row
        int cur_min, cur_min_index, prev_index;
        prev_index = (*path)[height]; //save the MID location of the previous path
        
        //left edge, only compare index 0 and 1 (mid and right)
        if (prev_index == 0){
            cur_min = best[(height-1)*(width)]; //the mid 
            cur_min_index = 0;
            cur_min = fmin(cur_min, best[(height-1)*(width) + 1]);
            if (cur_min == best[(height-1)*(width) + 1]){
                cur_min_index = 1;
            }
            (*path)[height-1] = cur_min_index; 
            recover_path_rec(best, height-1, width, path); 
        
        //right edge, only compare index width-2 and width-1 (left and mid)
        } else if (prev_index == width-1){
            cur_min = best[(height-1)*(width) + prev_index-1]; //the left side
            cur_min_index = prev_index-1;
            cur_min = fmin(cur_min, best[(height-1)*(width) + prev_index]);
            if (cur_min == best[(height-1)*(width) + prev_index]){
                cur_min_index = prev_index;
            }
            (*path)[height-1] = cur_min_index; 
            recover_path_rec(best, height-1, width, path); 
        } else {
            //store from left index, compare with mid and right
            cur_min = best[(height-1)*(width) + prev_index-1];
            cur_min_index = prev_index-1;
            for (int i = 0; i < 2; i++){
                cur_min = fmin(cur_min, best[(height-1)*width+prev_index+i]); //compare cur_min to current pixel
                if (cur_min == best[(height-1)*width+prev_index+i]){ //if cur_min IS the current then save the index
                    cur_min_index = prev_index+i;
                }
            }
            (*path)[height-1] = cur_min_index; //store in array
            recover_path_rec(best, height-1, width, path); //send recursion next line above
        }
    }
}

void recover_path(double *best, int height, int width, int **path){
    (*path) = (int *)malloc(height * sizeof(int));
    memset(*path, 0, height * sizeof(int));

    recover_path_rec(best, height, width, path);
}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    create_img(dest, src->height, src->width - 1); //dest is one width less bc we're removing one pixel each time
    memset((*dest)->raster, 0, 3*((*dest)->height * (*dest)->width * sizeof(uint8_t)));
    int d_width = (*dest)->width; //should be 1 smaller than src
    int s_width = src->width;

    for (int y = 0; y < src->height; y++){
        memmove(&((*dest)->raster[3*y*(d_width)]), &(src->raster[3*y*s_width]), path[y]*3*sizeof(uint8_t)); //copy all pixels after the seam
        memmove(&((*dest)->raster[3*(y*(d_width) + path[y])]), &(src->raster[3*((y*s_width)+(path[y] + 1))]), (d_width - path[y])*3*sizeof(uint8_t));
    }
}