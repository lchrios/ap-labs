#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINES 10000

void mergeSort(void *array[], int l, int r, int (*compare)(void *, void *));
void merge(void *array[], int l, int m, int r, int (*compare)(void *, void *));
int intcmp(char *, char *);

char *lines[MAX_LINES];

int main(int argc, char** argv) 
{
	// default set for string
    int type = 0;

    if(argc < 2) {
        printf("Error: wrong input of parameters.\n");
        printf("Usage: ./generic_merge_sort <<type -n|default for strings>> <<fptrname>>\n");
        return 1;
    }

    if(argc == 2 && strcmp(argv[1],"-n") == 0) {
        printf("Error: wrong set of parameters.\n");
        printf("Usage: ./generic_merge_sort <<type -n|default is string>> <<fptrname>>\n");
        return 1;
    }

	// -n -> flag for numbers manage
    if(argc == 3 && strcmp(argv[1],"-n") == 0) {
        type = 1;
    }

    FILE *fptr;

	if((fptr = fopen(argv[type ? 2 : 1], "r")) == NULL) {
		printf("Error while opening fptr: %s", argv[1]);
		return 1;
	}
    
    char line[256] = {0};
    int i = 0;
    
	while(fgets(line, 256, fptr)) {
        lines[i] = (char*)malloc(strlen(line) + sizeof(char*));
        strcpy(lines[i],line);
        i++;
    }

    if(fclose(fptr))
        printf("Error while closing the fptr...\n");


    mergeSort((void *) lines, 0, i-1, (int (*)(void*, void*)) (type ? intcmp : strcmp));
	
	FILE *resf;

	if (type) {
		// write in document
		resf= fopen("mergedStr.txt", "w+");
		
		for (int j = 0; j < i; j++){
			printf("%s", lines[j]);
			fprintf(resf, "%s", lines[j]);
		}
	} else {
		// write in document
		resf = fopen("mergedNum.txt", "w+");
		
		for (int j = 0; j < i; j++){
			printf("%s", lines[j]);
			fprintf(resf, "%s", lines[j]);
		}
	}

	fclose(resf);

    return 0;
}

void mergeSort(void* array[], int l, int r, int (*compare)(void *, void *)) {
    if (l < r) { 
        int m = l+(r-l)/2; 

        mergeSort(array, l, m, compare); 
        mergeSort(array, m+1, r, compare); 

        merge(array, l, m, r, compare); 
    } 

}

void merge(void* array[], int l, int m, int r, int (*compare)(void *, void *)){

    int i, j, k, 
		top_i = m-l + 1, 
		bot_i = r - m; 

    void *top[top_i], *bot[bot_i]; 

    for (i = 0; i < top_i; i++)
        top[i] = array[l + i];

    for (j = 0; j < bot_i; j++) 
        bot[j] = array[m + 1+ j]; 

    i = 0; 
    j = 0; 
    k = l; 

    while (i < top_i && j < bot_i) { 

        if (compare(top[i], bot[j])<=0) { 
          
		    array[k] = top[i]; 
            i++; 
        
		} else { 
        
		    array[k] = bot[j]; 
            j++; 
        
		} 
        k++; 
    } 
    while (i < top_i) {

        array[k] = top[i]; 
        i++; 
        k++; 

    } 
    while (j < bot_i) {

        array[k] = bot[j]; 
        j++; 
        k++;

    }
}


int intcmp(char *s1, char *s2) {

    double v1 = atof(s1), 
		   v2 = atof(s2);

    if (v1 < v2)
        return -1;
    else if(v1 > v2)
        return 1;
    else
        return 0;

}