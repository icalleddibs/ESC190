// Lab 1

#include <stdio.h>

//Question 2
void change(int *a){ //send over the address of a
    int *p_a = a; //p_a points to the address of a
    *p_a = 10; //changes the value in the address of a to 10
}

//Question 3
void change_arr(char *array){
    array[1] = 'd';
}

//Question 4
void swap(int arr[], int length){
    int temp = 0;
    for (int i = 0; i < length; i++){
        for (int j = i+1; j < length; j++){
            if (arr[i] > arr[j]){
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

//Question 5
int str_len(char *string){
    int len = 0;
    int i = 0;
    for (i = 0; string[i] != 0; i++){
        len++;
    }
    return len;
}

//Question 6
void seq_replace(int *arr1, unsigned long arr1_sz, int *arr2, unsigned long arr2_sz){
    for (int i = 0; i <= (arr1_sz - arr2_sz); i++){
        int j;
        for (j = 0; j < arr2_sz; j++){
            if (arr1[i+j] != arr2[j]){
                break;
            }
        }
        if (j == arr2_sz){
            for (j = 0; j < arr2_sz; j++){
                arr1[i+j] = 0;
            }
        }
    }
}


void main()
{
    //Question 2
    int a = 5;
    printf("Original Value: %d\n", a);
    change(&a);
    printf("New Value: %d\n", a);

    //Question 3
    char array[] = "abc";
    printf("Original array: %s\n", array); 
    change_arr(array);
    printf("New array:%s\n", array);

    //Question 4
    int i = 0;
    int arr[5] = {6, 4, 5, 7, 2};
    int length = (sizeof(arr)/sizeof(arr[0]));
    printf("Unsorted: "); 
    for (i = 0; i < length; i++){
        printf("%d", arr[i]); 
    }
    printf("\n");

    swap(arr, length);

    printf("Sorted: "); 
    for (i = 0; i < length; i++){
        printf("%d", arr[i]); 
    }
    printf("\n");

    //Question 5
    char *string = "hello";
    int len = str_len(string);
    printf("Length of string: %d\n", len);

    //Question 6
    int arr1[] = {5, 6, 7, 8, 6, 7};
    int arr2[] = {6, 7};
    printf("Current array: ");
    i = 0;
    for (i = 0; i < 6; i++){
        printf("%d", arr1[i]);
    }
    printf("\n");
    seq_replace(arr1, 6, arr2, 2);
    printf("New array: ");
    i = 0;
    for (i = 0; i < 6; i++){
        printf("%d", arr1[i]);
    }


}