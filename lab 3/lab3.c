#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Question 1
void set_int1(int x){
    x = 42;
}
//it is sent in a copy of variable x, it will only change the local copy stored in local memory within this function

void set_int2(int *p_x){
    *p_x = 42;
}
//it is sent a pointer to the address of x in main, and we are changing the value (*p_x) at the address
//so it changes the value at the main

//Question 2
typedef struct student1{
    char name[200];
    char student_number[11];
    int year;
} student1;

void print_sinfo(student1 s1){
    printf("Q2 student name: %s\n", s1.name);
    printf("Q2 student number: %s\n", s1.student_number);
    printf("Q2 year: %d\n", s1.year);
    //items are defined in main using strcpy
    //so just send the value in (no address) and print
}

//question 3a
void set_default_name(student1 *p_s){
    strcpy(p_s->name, "Default Student"); //same as (*p_s).name, only with addresses
    p_s->year = 0;
}

//question 3b
void set_default_name_wrong(student1 s){
    strcpy(s.name, "Default Student Wrong");
    //printf("%s\n", s.name); //showing that it only changes local s.name. it prints as wrong here
    s.year = 0;
    //printf("%d\n", s.year); //only works locally, not in main
    //even though it's not an array, you are not sending the address so it won't work
}

//question 4
//Write a function that uses malloc that takes in a pointer to student * and sets the pointer to the address
//of a block of n students students1’s.
void create_block1(student1 **p_p_s, int n_students){
    *p_p_s = (student1 *)malloc((sizeof(student1)) * (n_students));
    //so the value of p_p_s (*p_p_s) is the address of the block
    //the value of *p_p_s (**p_p_s) is the address
    //*p_p_s is going to point to an address that has a block with enough sections for n times the size of struct
    //and the size of one struct the size of all its members
    //the size of one student1 is 216 because 200+11+4+null = 216
    //for some reason it says the size of **p_p_s is also 216 but *p_p_s is 8
}

//question 5
void set_name(student1 **p_p_s, student1 *p_s, char *name){
    //set the name of student1 to a string
    //the string is given into the function not literal scan input
    //make a loop to go through and save the first 199 characters, set the 200th to null
    char validstr[200];
    int i;
    if (strlen(name) > 199){
        for (i = 0; i < 199; i++){
            validstr[i] = name[i];
        }
        validstr[i] = '\0';
        strcpy(p_s->name, validstr);
    } else if (strlen(name) > 0){
        strcpy(p_s->name, name);
    }
    **p_p_s = (*p_s);
}

//question 6
void destroy_block1(student1 **p_p_s){ //only need address not the value so single pointer ok?
    free(*p_p_s);
}

//question 7
typedef struct student2{
    char *name;
    char *student_number;
    int year;
} student2;

void create_block2(student2 **p_p_s, int num_students){
    *p_p_s = (student2 *)malloc(sizeof(student2) * num_students);
    int i;
    student2 *temp = *p_p_s;
    for (int i = 0; i < num_students; i++){
        temp->name = 0;
        temp->student_number = 0;
    }
}

//question 8
void set_name2(student2 *p_p_s, char *name){
    //p_s = (student2*)malloc((strlen(name)+1) * sizeof(char));
    //strcpy(temp->name, name);
    //strcpy(p_s->name, name);
    //**p_p_s = *p_s;
    //p_s->name = name;
    //printf("%d\n", p_s);
    //printf("%s", *p_s);
    //**p_p_s = (*p_s);

    p_p_s->name = malloc(strlen(name) + 1);
    strcpy(p_p_s->name, name);
    printf("%s\n", p_p_s->name);
}

//question 9
void destroy_block2(student2 **p_p_s, int n_students){ //only need address not the value so single pointer ok?
    student2 *temp = *p_p_s;
    int i;
    for (i = 0; i < n_students; i++){
        if (0 != temp->name){
            free( temp->name);
            temp->name = 0;
        }
        if (0 != temp->student_number){
            free( temp->student_number);
            temp->student_number = 0;
        }
        temp++;
    }
    free(*p_p_s);
    *p_p_s = 0;
}





//---------------------------------------------------------------------------------------------------

void main()
{
    //question 1
    int x = 2;
    set_int1(x);
    printf("Q1 %d\n", x);
    set_int2(&x);
    printf("Q1 %d\n", x);

    //question 2
    student1 s1;
    /*
    s1.name = "diba";
    s1.student_number = "420";
    */
    strcpy(s1.name, "diba");
    strcpy(s1.student_number, "420");
    s1.year = 2023; //works because it's not an array
    print_sinfo(s1);

    //question 3
    set_default_name(&s1);
    printf("Q3 new name: %s\n", s1.name); //yes it got changed because it modified this address
    printf("Q3 new year: %d\n", s1.year);
    set_default_name_wrong(s1);
    /*
    printf("wrong name: %s\n", s1.name); //nothing changed because main address wasn't sent or modified
    printf("wrong year: %d\n", s1.year);
    doesn't show as wrong year now when you print because the correct function already did it
    but when you run this before correct function, it prints original values, so didn't work.
    */

    //question 4
    student1 *block;
    int n = 2;
    create_block1(&block, n);
    // you have to send it the address of a pointer
    //printf("%d", sizeof(*mystr)); //it's still 216?

    //question 5
    student1 sq5;
    char *name = "Q5 hello";
    set_name(&block, &sq5, name);
    printf("%s\n", block);

    //question 6
    destroy_block1(&block);
    //when you print you get undefined behaviour which means free worked to clear the data

    //question 7
    student2 *block2;
    int n2 = 2;
    create_block2(&block2, n2);

    //question 8
    //student2 sq8;
    char *name2 = "Q8 test";
    set_name2(block2, name2);
    printf("%s\n", block2);

    //question 9
    destroy_block2(&block2, n2);
}