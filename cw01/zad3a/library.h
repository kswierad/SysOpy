#ifndef ZAD1_LIBRARY_H
#define ZAD1_LIBRARY_H


#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#define MAX_WIDTH 10000
#define MAX_LENGTH 100000

extern char staticArray[MAX_LENGTH][MAX_WIDTH];
typedef struct WrappedArray{
    int is_static;
    char **array;
    int height;
    int width;
}WrappedArray;

WrappedArray* create_array(int height, int width, int is_static);
void delete_array(WrappedArray* array);
void create_block(WrappedArray* array, char* value, int index);
void delete_block(WrappedArray* array, int index);
char* find_block(WrappedArray* array, int key);
int calc_block(char* block);


#endif