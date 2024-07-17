/*
implement a heap that can store the integer values in c
make sure to realloc if there is no space
implement c, header, and test code :(
*/

/*
heap = {1, 2, 3, 4, 5, 6, 7, 8}
n = 8
        1   
        /\
       2 3
      /\ /\
     4 5 6 7
    /\ /\ /\ /\
   8 

*/

void heapify(int *heap, int i, int size){
    int left = 2*i + 1;
    int right = 2*i + 2;
    int largest = i;
    if(left < size && heap[left] > heap[largest]){
        largest = left;
    }
    if(right < size && heap[right] > heap[largest]){
        largest = right;
    }
    if(largest != i){
        int temp = heap[i];
        heap[i] = heap[largest];
        heap[largest] = temp;
        heapify(heap, largest, size);
    }
}

void buildHeap(int *heap, int size){
    int i;
    for(i = size/2; i >= 0; i--){
        heapify(heap, i, size);
    }
}

void main(){
    int heap[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size = 8;
    buildHeap(heap, size);
}