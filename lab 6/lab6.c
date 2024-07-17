#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//somehow do array list but it points at itself again? 
//ok but array lists dont point they just go to next position...
// make an array list
// take the solutions from him
// but you need to make enqueue and dequeue functions
// if enqueue is greater than size then need to realloc

// so i need to make a STRUCT
// cqueue-> size = size
// cqueue-> capacity = capacity, if size >= capacity must realloc
// cqueue-> begin = changes to point to current beginning
// cqueue-> end = changes to point to current ending
// cqueue-> data = array of data

// basically we can use cqueue->data[i] to change stuffs based on head and tail
// same way of incrementing and doing if statements as what we did in python
// you advance the address by one so + 1 index 
// we need to permanently keep track of head separately from begin of numbers



// do this in the morning you are tired even though it's 10:23pm
// get rest
// you can do this
// i believe in you
// i love you :)
// gotta love yourself more you idiot (affectionate)


typedef struct CircularQueue{
    int *data; //this is the HEAD aka FIRST EVER POSITION
    int size;
    int capacity;
    int begin; //will hold indexes
    int end; //to position in data?
} CircularQueue;

// just create the queue
void create_CQ_from_data(CircularQueue **p_CQ, int *data_arr, int size)
{
    (*p_CQ) = (CircularQueue*)malloc(sizeof(CircularQueue));
    (*p_CQ)->size = size;
    (*p_CQ)->capacity = size;
    (*p_CQ)->begin = 0;
    (*p_CQ)->data = (int*)malloc(sizeof(int) * size);
    for(int i = 0; i < size; i++){
        (*p_CQ)->data[i] = data_arr[i];
    }
    (*p_CQ)->end = (size-1);
}

void CQ_enqueue(CircularQueue *my_list, int new_elem)
{
    if(my_list->size == my_list->capacity){
        my_list->capacity *= 2;
        //make a new list of data?
        int *temp;
        temp = (int*)malloc(sizeof(int) * my_list->size);
        int i = 0;
        //for (int i = 0; i < my_list->size; i++){
        for (int j = my_list->begin; j < my_list->size; j++){
            temp[i] = my_list->data[j];
            i++;
        }
        for (int k = 0; k < my_list->begin; k++){
            temp[i] = my_list->data[k];
            i++;
        }
        
        my_list->data = (int*)realloc(temp, sizeof(int) * my_list->capacity);
        //OKAY when the list is full and we need to allocate new space
        //we should restart a new list and reset the indices
        //so the allocated space will contain all the num from my_list->begin to 
        //whatever the end is, so 
        //size - begin? 6 - 1 so index 5
        //do this before changing size
        my_list->begin = 0;
        my_list->end = my_list->size - 1;
        my_list->data[my_list->end + 1] = new_elem;
        my_list->size++;
        my_list->end++;
    } else if (my_list->end == my_list->capacity - 1){
        my_list->end = 0;
        my_list->data[my_list->end] = new_elem;
        my_list->size++;
    } else {
        my_list->data[my_list->end + 1] = new_elem;
        my_list->size++;
        my_list->end++;
    }
}
//THIS MIGHT BE FOR DEQUEUE
//if my_list->begin  == (my_list->size) - 1
        //then place at 0
        //change end to 0
    //else if my_list-> begin == NULL && my_list->end == 0
        //put it at my_list->data[0] because this is the start, nothing has been placed in yet
    //else
        //(my_list->end)++ should increase the NUMBER VALUE by 1
        //my_list->data[my_list->end] = new_elem;


// dequeue needs to do this but you know... not move everything up?
void CQ_dequeue(CircularQueue *my_list)
{
    //no more memove, just set that position to null?
    //set begin to null
    my_list->data[my_list->begin] = 1000;
    my_list->size--;
    my_list->begin++;
    
    //memmove(my_list->data + index, my_list->data + index + 1, sizeof(int) * (my_list->size - index - 1));
    //my_list->size--;
}

void CQ_print(CircularQueue *my_list){
    for(int i = 0; i < my_list->size; i++){
        printf("%d ", my_list->data[i]);
    }
    //this is not done bc i need it to only print from a certain index.
}

void CQ_free(CircularQueue *my_list)
{
    free(my_list->data);
    free(my_list);
}


//-------------------------------------------------------------
void main(){
    CircularQueue *p_list;
    int data_arr[] = {1, 2, 3};
    int size = 3;
    create_CQ_from_data(&p_list, data_arr, size); 

    CQ_enqueue(p_list, 6);
    CQ_enqueue(p_list, 7);
    CQ_enqueue(p_list, 8);
    CQ_dequeue(p_list);
    CQ_enqueue(p_list, 9);
    CQ_enqueue(p_list, 10);
    CQ_enqueue(p_list, 11);
    CQ_dequeue(p_list);
    CQ_enqueue(p_list, 12);
    CQ_enqueue(p_list, 13);
    CQ_dequeue(p_list);
    CQ_enqueue(p_list, 14);
    CQ_enqueue(p_list, 15);
    CQ_dequeue(p_list);
    CQ_dequeue(p_list);
}


