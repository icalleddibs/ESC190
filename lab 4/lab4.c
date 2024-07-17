#include <stdio.h>
#include <string.h>
#include <math.h>

//Q1 a, b
void change(int *p){
    p[100000] = 5;
}

//went into wsl terminal.
//gcc lab4.c
//valgrind ./a.out
//it said segmentation fault!!

/*
typedef struct student1{
    char name[3];
    int age;
} student1;

void change_name1_wrong(student1 s){
    s.name[0] = 'b';
}

void change_name1_right(student1 *p_s){
    p_s->name[0] = 'b';
}

void change_name1_right2(student1 *p_s){
    strcpy(p_s->name, 'b');
}
*/

/* C MEMORY TABlE
32 |
36 |'b', 'b', '\0', [...] //s local copy, doesn't change the original
40 |
44 |
48 | 100 // p_s, (a) can change the original through addressing
52 |
56 |
60 | 100 // p_s, (b) can change the original through copying to address
64 |
68 |
72 |
76 |
80 |
84 |
88 |
92 |
96 |
100|'a', 'b', '\0', [...] //s
104|
108|
112|
116|
120|
124|
*/

int * sum_of_constants(int *p_a){
    //send in a which will just be 0 that we can rewrite
    FILE *fp = fopen("a.txt", "r");
    char line[200];

    double sum = 0;
    int res = 0;
    int above = 0;
    int below = 0;
    int i = 0;
    int j = 0; //until decimal
    int k = 0; //uneil full number

    while (fgets(line, sizeof(line), fp)){
        for (int i = (strlen(line) - 2); i > 0; i--){
            if (line[i] != '.'){
                j++;
            } else {
                break;
            }
        }
        for (int i = (strlen(line) - j - 3); i > 0; i--){
            if (line[i] != ' '){
                k++;
            } else {
                break;
            }
        }
        //below = my_atoi(line[(strlen(line)) - j]);
        char *str = line[1];
        printf("%s", str);
        /*
        for (int i = (strlen(line) - 2); i > 0; i--){ //maybe -2 idk i dont want null
            if (line[i] != '.'){
                res = my_atoi(line[i]);
                res = res * pow(10, j);
                sum = sum + res;
                j++;
            } else if (line[i] == '.'){
                sum = sum * pow(10, -j);
            }
            //do it again? after the point until a space
        }
        printf("%d", sum);
        */
    }

    fclose(fp);
    *p_a = sum;
    return p_a;
}

int my_atoi(const char *str){
    int i = 0;
    //int sign = 1;
    //if(str[i] == "-"){
    //    sign = -1;
    //    i++;
    //}
    int result = 0;
    while(str[i] != '\0'){
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

void main(){
    int p[] = {1};
    //change(&(*p));

    //Q1 c
    FILE *fp = fopen("a.txt", "r");
    if (fp == NULL){
        printf("File cannot be opened");
    } else {
        char line[100000];
        fgets(line, 100000, fp);
        printf("%s", line);
        fclose(fp); //remove this for memory leak
    }
    //Q1 d

    printf("%s", fp);
    //fp2 printed as null! the file cannot be opened

    int a = 0;
    int res = sum_of_constants(&a);
    printf("%d", res);
}