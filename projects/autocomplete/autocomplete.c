#if !defined(AUTOCOMPLETE_H)
#define AUTOCOMPLETE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct term{
    char term[200]; // assume terms are not longer than 200
    double weight;
} term;

// PART 1 --------------------------------------------------------------------------------------------

//read num lines function
int read_num_lines(char *filename){
    //the first line of the file is the number of terms
    //but this is practice for a loop to make sure its correct
    //in case a test file does not have the first line
    //so it's num_lines - 1
    FILE *fp = fopen(filename, "r");
    char line[200];
    int num_lines = 0;
    while (fgets(line, 200, fp) != NULL){
        num_lines++;
    }
    fclose(fp);
    return num_lines;
}

//compare function for qsort increasing term name
int compare_terms (const void *p_a, const void *p_b){
    term *a = (term *)p_a;
    term *b = (term *)p_b;
    //compare the term names using strcmp
    return strcmp(a->term, b->term);
}


/* The function allocates memory for all the terms in the file and stores a pointer to the block in
*terms. The function stores the number of terms in *pnterms. The function reads in all the terms
from filename, and places them in the block pointed to by *terms.
The terms should be sorted in ascending lexicographic order. */
void read_in_terms(term **terms, int *pnterms, char *filename){
    //get number of lines
    int num_lines = (read_num_lines(filename) - 1); //ignoring first line
    //num_lines is also simply given as first line in file
    *pnterms = num_lines;

    //allocate memory
    *terms = (term *)malloc(sizeof(term) * num_lines);
    memset(*terms, 0, sizeof(term) * num_lines); //set to 0
    term *temp = *terms; //manipulate address of temp instead of losing terms address
   
    FILE *fp = fopen(filename, "r");
    char line[200];
    char newline[200]; //for restoring line after spaces
    int spaces = 0;
    int i;
    char *token;
    fgets(line, 200, fp); //getting rid of first line (it is also the # of terms)

    //storing all items in *terms
    for (int j = 0; j < num_lines; j++){
        fgets(line, 200, fp);
        for (i = 0; line[i] == ' '; i++){
            spaces++;
        }
        strcpy(newline, &(line[spaces])); //save everything after the spaces
        spaces = 0; //so it begins again for every line
        newline[strlen(newline)-1] = '\0';

        token = strtok(newline, "\t");                 //split on tab
        if (token != NULL){
            long long int weight = atoll(token);    //this was to fix huge numbers
            weight = (double)weight;                //idk if this is right. still decimals.
            temp[j].weight = weight;                //place in struct j of temp 

            token = strtok(NULL, "\t");
            strcpy((temp[j].term), token);          //assume less than 200
        }
    }
    fclose(fp);

    //sort in ascending lexicographical order
    qsort(temp, num_lines, sizeof(term), compare_terms);
}

// PART 2 --------------------------------------------------------------------------------------------

int low_binary_search(term *terms, int low, int high, char *substr){
    int mid = (low + high) / 2;
    if (low == high){ //keeps going until low and high are the same (lowest match with substr)
        return mid;
    } else {
        //strncmp will allow you to check a certain number of chars (only how much substr is)
        if (strncmp(terms[mid].term, substr, strlen(substr)) < 0){ 
            // means terms comes before substr alphabetically, so we need to go higher
            return low_binary_search(terms, mid + 1, high, substr);
        } else {
            // means terms comes after substr alphabetically, so we need to go lower
            return low_binary_search(terms, low, mid, substr);
        }
    }
}

/* The function returns the index in terms of the FIRST TERM in lexicographic ordering that matches
the string substr. Must run in O(log(nterms)) time, where nterms is the number of terms in terms.
You can assume that terms is sorted in ascending lexicographic order.*/
int lowest_match(term *terms, int nterms, char *substr){
    int low = 0;
    int high = nterms - 1;
    int index = 0; 

    index = low_binary_search(terms, low, high, substr);
    //printf("low index: %d", index);
    return index;
}


// PART 3 --------------------------------------------------------------------------------------------

int high_binary_search(term *terms, int low, int high, char *substr){
    int mid = (low + high) / 2;
    if (low == high){
        // -1 because it will give us the first term w next letter in alphabet, so move back 1 position
        return mid - 1;
    } else {
        if (strncmp(terms[mid].term, substr, strlen(substr)) <= 0){
            // if terms comes before (or same as) substr alphabetically, we need to go higher
            // so it considers the next letter in the alphabet
            return high_binary_search(terms, mid + 1, high, substr);
        } else {
            // if terms comes after substr alphabetically, we need to go lower
            return high_binary_search(terms, low, mid, substr);
        }
    }
}


/* The function returns the index in terms of the LAST TERM in lexicographic order that matches
the string substr. Must run in O(log(nterms)) time, where nterms is the number of terms in terms.
You can assume that terms is sorted in increasing lexicographic order.*/
int highest_match(struct term *terms, int nterms, char *substr){
    int low = 0;
    int high = nterms - 1;
    int index = 0; 

    index = high_binary_search(terms, low, high, substr);
    //printf("high index: %d", index);
    return index;
}


// PART 4 --------------------------------------------------------------------------------------------

//compare function for qsort decreasing weight
int compare_weights(const void *p_a, const void *p_b){
    term *term1 = (term *)p_a;
    term *term2 = (term *)p_b;

    // returning -1 actually says a is less than b but we are doing decreasing order
    if (term1->weight > term2->weight){
        return -1;
    // returning 1 actually says a is greater than b but we are doing decreasing order
    } else if (term1->weight < term2->weight){
        return 1;
    } else {
        return 0;
    }
}

/* The function takes terms (assume lexicographically increasing), the number
of terms nterms, and the query string substr, and places the answers in answer, with *n_answer
being the number of answers. The answers should be sorted by weight in decreasing order.
MUST USE QSORT */
void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr){
    //get the indexes of the lowest and highest match within terms
    int low = lowest_match(terms, nterms, substr);
    int high = highest_match(terms, nterms, substr);
    *n_answer = (high - low + 1); //number of answers

    //if there are no matches, set n_answer to 0 and answer to NULL
    if (*n_answer <= 0){
        *answer = NULL;
    //allocate memory for the answers
    } else {

        *answer = (term *)malloc(sizeof(term) * (high - low + 1));
        memset(*answer, 0, sizeof(term) * (high - low + 1)); //set all values to 0

        for (int i = low, j = 0; i <= high; i++, j++){
            (*answer)[j] = terms[i];
        }

        qsort(*answer, *n_answer, sizeof(term), compare_weights); //sorts in decreasing weight order
    }
}

/*
void main(){
    struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities.txt");
    lowest_match(terms, nterms, "Tor");
    highest_match(terms, nterms, "Tor");
    
    struct term *answer;
    int n_answer;
    autocomplete(&answer, &n_answer, terms, nterms, "Tor");
    //free allocated blocks here -- not required for the project, but good practice
}
*/



#endif