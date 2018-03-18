#include "library.h"
#include <stdio.h>

WrappedArray* create_array(int height, int width, int is_static){
    WrappedArray* result = malloc(sizeof(WrappedArray));
    result->height = height;
    result->width = width;
    result->is_static = is_static;
    if(is_static){
        result->array =(char**) staticArray;
    } else {
        result->array = calloc(height, sizeof(char*));
    }
    return result;
}

void delete_array(WrappedArray* array){
    if(array->is_static){
        array->width=0;
        array->height=0;
    } else {
        array->width=0;
        array->height=0;
        for (int i = 0; i < array->height; i++) {
            if (array->array[i] != NULL) free(array->array[i]);
        }
        free(array);
    }
}

void create_block(WrappedArray* array, char* value, int index){
    if(index>array->height || index<0 || array->array[index]!=NULL || strlen(value)>array->width) return;
    else {
        if(array->is_static) strcpy(array->array[index], value);
        else {
            array->array[index] = calloc(strlen(value), sizeof(char));
            strcpy(array->array[index],value);
        }
    }
}

void delete_block(WrappedArray* array, int index){
    if(array->is_static){
        for(int i=0;i<strlen(array->array[index]);i++) array->array[index][i]=0;
    } else {
        array->array[index]= NULL;
    }
}
char* find_block(WrappedArray* array, int key){
    char* result = NULL;
    int min_diff = INT_MAX;
    for(int i =0; i < array->height; i++){
        char* tmp = array->array[i];
        if(abs(calc_block(tmp)-key)<min_diff){
            min_diff = abs(calc_block(tmp)-key);
            result = tmp;
        }
    }
    return result;
}
int calc_block(char* block){
    int result = 0;
    for (int i=0; i < strlen(block); i++) result = result + block[i];
    return result;
}
