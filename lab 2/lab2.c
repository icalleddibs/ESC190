#include <stdio.h> //for print
#include <stdlib.h> //for scan
#include <ctype.h> //for isdigit
#include <math.h> //for pow
#include <string.h> //for strlen


//QUESTION 1
void get_string(char **p_s, int *p_n){

    printf("Number of characters in the string: ");
    scanf("%d", p_n);
    //scan must be given an address to store in (&n -> *p_n)

    int i;
    *p_s = (char *)malloc((*p_n) * (sizeof(char) + 1));
    //creating the memory block for address of str based on size

    for (i = 0; i < *p_n; i++){ //less than n characters bc start at 0
        printf("Type the next character: ");
        scanf(" %c", &(*p_s)[i]);
        //will put the scan in the address of memory block at [i]
        //must do " %c" or "%s" because "enter" counts as something to scan
    }
    (*p_s)[i] = '\0';
    //must add NULL at the end to be a valid string
}


//QUESTION 2: concatenate the strings.
//QUESTION 2A) In one version, use the index i to access elements of strings.
char *strcat1(char *dest, char *src){ //missing mallocs
    int i;
    int j;
    for(i = 0; dest[i] != '\0'; i++);
    // this will make it run until dest is complete so the i goes to the length of dest without null
    for(j = 0; src[j] != '\0'; j++){
        dest[i+j] = src[j];
        // just add src on to the end of dest
    }
    dest[i+j] = '\0'; //add null
    return dest;
    
}

//QUESTION 2B) In another version, only use pointer arithmetic.
char *strcat2(char *dest, char *src){
    char *old_dest = dest + strlen(dest); //give old_dest the address of dest, add the length which is how many bites
                                          //then it adds that many spaces in memory! and knows where to start adding src
    while (*src != '\0'){
        *old_dest++ = *src++; //put address of src into dest and increment BUT the increment means address changes
                              //so og address of dest start will be lost, save in old_dest
    }
    *old_dest = '\0';
    return old_dest; //old address of dest to make sure it doesn't get lost while incrementing
}


//QUESTION 3
/*
Explain the difference between s1 == s2, *s1 == *s2, and strcmp(s1, s2).
s1 == s2 won't even work because you are not accessing the actual content of the strings
         and the boolean operation has nothing to compare. so it probably crashes
*s1 == *s2 NOT SURE will check to see if the address in memory is the same location?
strcmp(s1, s2) will check to see if the contents of the string, aka the values in memory are the same
*/
int strcmp(const char *str1, const char *str2){

    //base condition: got to the end, same length, all same char so far, for sure 0 identical
    if(*str1 == '\0' && *str2 == '\0'){
        return 0;
    }
    //base condition: at any point, they are not equal OR one reaches end earlier (not same size)
    if(*str1 != *str2 || *str1 == '\0' || *str2 == '\0'){
        return 1;
    }
    //if they are same so far, keep going with next char
    //remember, i++ str1[i] SAME AS str1 + 1, goes to next space in mem until null
    else{
        return strcmp(str1 + 1, str2 + 1);
    }
}


// QUESTION 5
// convert string of numbers to an actual integer
int my_atoi(char *string){
    int length = strlen(string); // find the length for knowing the power
    int i;
    int res = 0;

    for (i = 0; string[i] != '\0'; i++){
        if(isdigit(string[i])){
            res += (string[i] - '0') * pow(10, (length - i - 1));
            //starting with the first number, it should be 10000 + 2000 + 300 + 40 + 5 = 12345
            //so do 10^ (length - i - 1) because 10000 = 10^4, length is 5, - 1 extra.
        }
    }
    return res;
}


//------------------------------------------------------------------------------------------------------

void main()
{
    //Q1
    char *my_str;
    int n;
    get_string(&my_str, &n);
    //address of address of my_str, address of n

    printf("Print Q1: %s\n", my_str);
    //my_str is an address so it will already print from the address until null
    //we put stuff in the memory that this address points to so it's accessible
    free(my_str);
    //clear up the memory block at this address

    //Q2a
    char dest[10000] = "hello";
    char src[10] = " world";
    strcat1(dest, src);
    printf("Print Q2a: %s\n", dest);

    //Q2b
    char dest1[] = "hi";
    char src1[] = " ppl";
    strcat2(dest1, src1);
    printf("Print Q2b: %s\n", dest1);

    //Q3
    char str1[] = "ab";
    char str2[] = "ab";
    int compare;
    compare = strcmp(str1, str2);
    printf("Results of Q3: ");

    if (compare == 0){
        printf("The two strings are identical. \n");
    }
    else{
        printf("The two strings are different. \n");
    }

    //Q5
    char numstr[] = "420";
    int res = my_atoi(numstr);
    printf("Integer form of Q5: %d", res);
}