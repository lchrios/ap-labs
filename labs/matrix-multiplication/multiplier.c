#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "logger.h"

// constant
#define THREADS_NUM 2000

long **buffers;
int NUM_BUFFERS = 0;
char *resultMatrix;

pthread_mutex_t *mtx;
pthread_t threads[THREADS_NUM];

struct params {
    int row;
    int col;
    long *matA;
    long *matB;
};

long * getRow(int row, long *matrix){

    long *res = malloc(THREADS_NUM * sizeof(long)); // Allocation for resulting row
    int rowStart = THREADS_NUM * row; // offset to get to the required row

    // Saves all the 2000 results
    for(int i = 0; i < THREADS_NUM; i++){ res[i] = matrix[rowStart+i]; }

    return res;
}

long * getColumn(int col, long *mat){

    long *res = malloc(THREADS_NUM * sizeof(long)); // Allocation for the resulting column
    
    // Saves every column to the 
    for(int i = 0; i < THREADS_NUM; i++; col += THREADS_NUM){ res[i] = mat[col]; }
    
    return res;
}

int getLock() {

    for(int i = 0; i < NUM_BUFFERS; i++){ if(pthread_mutex_trylock(&mtx[i])==0){ return i; } }
    return -1;

}

int releaseLock(int lock) { return pthread_mutex_unlock(&mtx[lock]); }

long * readMatrix(char *fname){

    int size = 0;
    FILE *fp =fopen(fname,"r");
    
    if(fp == NULL){
    
        errorf("Error: cannot open the file containing the matrix data.");
        exit(2);
    
    }

    char c;
    // Read every char
    while((c=fgetc(fp))!=EOF){
        // A number has finished reading 
        if(c=='\n')
            size++;
    
    }

    rewind(fp);
    
    long *matrix = malloc(size * sizeof(long));
    int index = 0;

    while(fscanf(f, "%ld", &matrix[index]) != EOF){ index++; }
    
    fclose(f);
    
    infof("Successfully read the matrix");

    return matrix;   

}

long dotProduct(long *vecA, long *vecB) {
    
    long prod = 0;
    
    for(int i = 0; i < THREADS_NUM; i++) {
    
        prod += (vecA[i] * vecB[i]);
    
    }

    return prod;

}

long performMult(struct params *data){

    int bufA = -1, bufB = -1;
    
    while(bufA == -1 || bufB == -1){
        
        if(bufA == -1){ bufA = getLock(); }
        
        if(bufB == -1){ bufB = getLock(); }

    }

    // obtaining rous and column to multiplyy
    buffers[bufA] = getRow(data->row, data->matA);
    buffers[bufB] = getColumn(data->col, data->matB);
    
    long res = dotProduct(buffers[bufA], buffers[bufB]);
    
    // liberating memory
    free(buffers[bufA]);
    free(buffers[bufB]);
    free(data);

    // releasing locks
    releaseLock(bufA);
    releaseLock(bufB);

    return res;
}

long * multiply(long *matA, long *matB){

    infof("Starting to multiplying matrices");
    
    long *result = malloc(THREADS_NUM*THREADS_NUM * sizeof(long));
    
    for(int i = 0; i < THREADS_NUM; i++){
        
        for(int j = 0; j < THREADS_NUM; j++){
            
            // Parameters saving into params struct
            struct params *ar = malloc(sizeof(struct params));
            ar -> row = i;
            ar -> col = j;
            ar -> matA = matA;
            ar -> matB = matB;
            
            // create the thread to multiply 
            pthread_create(&threads[j], NULL, (void * (*) (void *)) performMult, (void *)ar);
        
        }

        for(int j = 0; j < THREADS_NUM; j++){
            
            void *res;
            
            pthread_join(threads[j], &res);
            
            result[THREADS_NUM * j + i] = (long) res;
        
        }
    }
    
    infof("Matrices fully multiplied");
    
    return result;
}

int saveResult(long *result) { 

    FILE *file;
    file = fopen(RESULT_MATRIX_FILE, "w+");
    
    if(file==NULL){
        return -1;
    }
    
    int size=THREADS_NUM*THREADS_NUM;
    
    // write all info
    for(int i = 0; i < size; i++) { fprintf(file, "%ld\n", result[i]); }
    
    fclose(file);
    
    return 0;
}


// Main

int main(int argc, char **argv)
{
    initLogger("stdout");

    if (argc < 5) {
    
        errorf("Error: Not enough parameters\n");
        errorf("The correct usage: ./multiplier -n <buffers> -out <file>\n");
        return -1;
    
    }

    int pc = 0; // counts both parameters had been saved
    for (int i = 0; i < argc; i++) {

        if (strcmp(argv[i], "-n")) { NUM_BUFFERS = atoi(argv[i++]); pc++; }
    
        if (strcmp(argv[i], "-out")) { NUM_BUFFERS = atoi(argv[i++]); pc++; }

    } 

    // couldnt 
    if (pc < 2) {
    
        errorf("Error: Not enough parameters\n");
        errorf("The correct usage: ./multiplier -n <buffers> -out <file>\n");
        return -1;
    
    }


    // variable initializaation
    buffers = malloc(NUM_BUFFERS * sizeof(long *));
    mtx = malloc(NUM_BUFFERS * sizeof(pthread_mutex_t));

    long *matA = readMatrix("matA.dat");
    long *matB = readMatrix("matB.dat");

    for(int i = 0; i < NUM_BUFFERS; i++){
        pthread_mutex_init(&mtx[i], NULL);
    }

    // operate
    long *res = multiply(matA, matB);
    
    saveResult(res);

    // liberate all memory used
    free(matA);
    free(matB);
    free(mtx);
    free(buffers);

    return 0;
}