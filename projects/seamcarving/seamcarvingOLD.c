#include "c_img.h" //but i dont need this for some reason?
#include "c_img.c" //check with dad idk why i need this for it to work
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

    //allocate memory for best arr first. has height*width pixels *double type
    *best_arr = (double *)malloc(grad->height * grad->width * sizeof(double));

    //first row: just keep top pixels
    for (int i = 0; i < width; i++){
        (*best_arr)[i] = get_pixel(grad, 0, i, 0); //doesnt matter what col bc all same energy
    }
    
    double cl, ca, cr, cost; //costs above
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
    //(*best_arr)[height*width] = 0; //last pixel is 0 DONT NEED
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
        memmove(&((*dest)->raster[3*y*d_width]), &(src->raster[3*y*s_width]), path[y]*3*sizeof(uint8_t)); //copy all pixels after the seam
        memmove(&((*dest)->raster[3*(y*(d_width) + path[y])]), &(src->raster[3*((y*s_width)+(path[y] + 1))]), (d_width - path[y])*3*sizeof(uint8_t));
    }
}

/*
void test_main(){
    struct rgb_img *im;
    struct rgb_img *grad;   //hold the dual energy gradients
    double *best;           //dynamic array for cost
    int* path;              //path to remove
    struct rgb_img *cur_im; //current image (dest)

    //initializing
    read_in_img(&im, "3x4.bin");

    //part 1
    calc_energy(im, &grad);
    print_grad(grad); //to print the gradient

    //part 2
    dynamic_seam(grad, &best);
    //print best_arr
    for (int i = 0; i < grad->height; i++){
        for (int j = 0; j < grad->width; j++){
            printf("%f ", best[i*grad->width+j]);
        }
        printf("\n");
    }

    //part 3
    //initialize path
    recover_path(best, grad->height, grad->width, &path);
    //print path
    for (int i = 0; i < grad->height; i++){
        printf("%d ", path[i]);
    }

    //part 4
    remove_seam(im, &cur_im, path);
}
*/
/*
    //PLANNING MEMMOVE!!!!!
    //---------------------------------------------------------------------------
    //width is 6 pixels so for every height y, there are 0 to width-1 indices

    //so (width)*height*3 is the new row
    //0 row: starts at 0  (3*6*0)
    //1 row: starts at 18 (3*6*1)
    //2 row: starts at 36 (3*6*2)
    //3 row: starts at 54 (6*3*3)
    //this works with the new formula bc index = 0
    //[3*(height*width + 0)] = [3*height*width]

    //so to access each individual pixel:
        //[0] 0, 1, 2
        //[1] 3, 4, 5
        //[2] 6, 7, 8
        //[3] 9, 10, 11
        //[4] 12, 13, 14
        //[5] 15, 16, 17
    //start of next row is 18
        //[6] 18, 19, 20
        //[7] 21, 22, 23 
    //the start of every pixel is index *3 btw for row 0
        //this works because [3*(0*6 + index)] = [3*index]

    // for everything else, it's [index in row]*height*width
    //so if you do im->raster[3*height*(6+index)]
    //[3*((2*6) + 0)] should give me the first part of third row

    //therefore the formula for accessing pixels is
    //[3*(height*6 + index in row)] where height starts at 0 row

    //so when im doing memmove, i want to keep everything from [3*(height*6)] (0 in the row)
    //until the beginning of the index given in path[height]
    //and memmove should already stop right before that location so we're good
    //first copy:
        //memmove ([3*(height*6)]  , [3*((height*6)+path[height])])

    //then we want to SKIP THAT PIXEL WITH INDEX path[height]
    //second copy:
        //needs to be into the next available spot in dest, but 1 pixel to the right in src
        //memmove (  [3*(height*6 + path[height])] , [3*((height*6)+(path[height] + 1))])
        //so dest should start at path[height] and src should start at path[height] + 1
    //WARNING: MIGHT NEED SPECIAL CASES FOR EDGES LIKE IF path[height] == s_width-1?
        //if the deleted thing is the last pixel of source (aka width-1)
        //ACTUALLY NO WE DONT! at least i dont think so, need to test
        //because last parameter is the amount of bytes...
    //amount of bytes
        //since now 
    // only copying pixel 0 is 0, 1, 2 = 3 bytes = 3*(index+1) bc 3*((1p-1)+1) = 3*1
    // copying pixel 0 & 1 is 0, 1, 2, 3, 4, 5 = 6 bytes = 3*((amt p)-1) bc 3*((2p-1)+1) = 3*2
    //so we now have to keep track of how many pixels we are copying
    //but since we are copying per row, we just need to know index of row
    //first copy is easy it's just path[length] 
        //ex. if path = 3, then it's actually copying 0, 1, 2 = path pixels = 9bytes 3*3
        //so it's 3*(path)
*/

//main from guerzhoy for printing tests
void main(){
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "HJoceanSmall.bin");
    //read_in_img(&im, "6x5.bin");
    
    for(int i = 0; i < 100; i++){
        printf("i = %d\n", i);
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        printf("%lu\n", im->height);
        printf("%lu\n", im->width);
        remove_seam(im, &cur_im, path);

        char filename[200];
        sprintf(filename, "img%d.bin", i);
        write_img(cur_im, filename);


        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }
    destroy_image(im);
}
