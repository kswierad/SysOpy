#include "library.h"

#include <stdio.h>

char staticArray[MAX_LENGTH][MAX_WIDTH];

WrappedArray* create_array(int height, int width, int is_static){

    WrappedArray* result = malloc(sizeof(WrappedArray));
    result->height = height;
    result->width = width;
    result->is_static = is_static;
    if(is_static){
        result->array = (char **) staticArray;
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

void create_block(WrappedArray* wrapped_array, char* value, int index){
    if(index>wrapped_array->height || index<0 || wrapped_array->array[index]!=NULL || strlen(value)>wrapped_array->width) return;
    else {
        wrapped_array->array[index] = calloc(strlen(value), sizeof(char));
        strcpy(wrapped_array->array[index],value);
    }
}

void delete_block(WrappedArray* array, int index){
    if(array->is_static){
        for(int i=0;i<array->width;i++) {
            if(array->array[index]!=NULL)
            array->array[index][i]=0;
        }
    } else {
        free(array->array[index]);
    }
}

char* find_block(WrappedArray* array, int key){
    char* result = NULL;
    int min_diff = INT_MAX;
    for(int i =0; i < array->height; i++){
        if(array->array[i]!=NULL) {
            char *tmp = array->array[i];
            if (abs(calc_block(tmp) - key) < min_diff) {
                min_diff = abs(calc_block(tmp) - key);
                result = tmp;
            }
        }
    }
    return result;
}
int calc_block(char* block){
    int result = 0;
    for (int i=0; i < strlen(block); i++) result = result + block[i];
    return result;
}