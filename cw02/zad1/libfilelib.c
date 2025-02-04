#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "libfilelib.h"

void generate_file(char *filePath, int recordsNumber, int recordSize){
    FILE *handle = fopen(filePath, "w+");
    //if(handle==NULL) printf("%s\n",filePath);
    FILE *rand_handle = fopen("/dev/urandom", "r");
    //if(rand_handle==NULL) printf("KURWA\n");
    unsigned char *buffer = calloc(recordSize+1, sizeof(char));
    if (handle && rand_handle){
        for (int i = 0; i < recordsNumber; i++){
            
            if(fread(buffer, sizeof(unsigned char),recordSize,rand_handle)!= recordSize){
            printf("Reading from /dev/urandom failed, shutting down.");
            fclose(handle);
            fclose(rand_handle);
            exit(1);
            }

            
            for (int j = 0; j < recordSize; j++){
                buffer[j] = buffer[j] % 52;
                if (buffer[j] < 26) buffer[j] = buffer[j] + 65;
                else buffer[j] = buffer[j] + 71;
            }
            buffer[recordSize-1]='\n';
            //printf("%s \n",buffer);
            if (fwrite(buffer, sizeof(unsigned char), recordSize, handle) != recordSize){
                printf("Writing to %s failed, shutting down.", filePath);
                fclose(handle);
                fclose(rand_handle);
                exit(1);
            }
        }
    } else {
        printf("Something went wrong.\n");
        //return;
    }
    fclose(handle);
    fclose(rand_handle);
}

void copy_file(char* sourceFileName, char* destFileName, int recordsNumber, int bufferSize){
    FILE* sourceFile = fopen(sourceFileName,"r");
    //if(sourceFile==NULL) printf("%s\n",sourceFileName);
    FILE* destFile = fopen(destFileName,"w");
    unsigned char* buffer = calloc(bufferSize,sizeof(char));
    if(sourceFile && destFile){
        for(int i=0; i < recordsNumber; i++){
            if(!fread(buffer,sizeof(char),bufferSize,sourceFile)){
                printf("Reading from %s failed, shutting down.",sourceFileName);
                fclose(sourceFile);
                fclose(destFile);
                exit(1);
            }
            if(!fwrite(buffer,sizeof(char),bufferSize,destFile)){
                printf("Writing to %s failed, shutting down.",destFileName);
                fclose(sourceFile);
                fclose(destFile);
                exit(1);
            }
        }
    } else {
        printf("Something went wrong.\n");
    }
    fclose(sourceFile);
    fclose(destFile);
}

void sort_file(char *filePath, int recordsNumber, int recordSize){
    FILE* handle = fopen(filePath,"r+");
    char* bufferOne = calloc(recordSize,sizeof(char));
    char* bufferTwo = calloc(recordSize,sizeof(char));
    if(handle){
        for(int i=1; i < recordsNumber; i++){
            fseek(handle,i*recordSize,0);
            fread(bufferOne,sizeof(char),recordSize,handle);
            int j=0;
            while(1){
                fseek(handle,j*recordSize,0);
                fread(bufferTwo,sizeof(char),recordSize,handle);
                if(j>=i || bufferOne[0]<bufferTwo[0]){
                    break;
                } 
                j++;
            }

            fseek(handle,-recordSize,1);
            fwrite(bufferOne,sizeof(char),recordSize,handle);
            for(int k = j+1; k < i+1; k++){
                fseek(handle,k*recordSize,0);
                fread(bufferOne,sizeof(char),recordSize,handle);
                fseek(handle,-recordSize,1);
                fwrite(bufferTwo,sizeof(char),recordSize,handle);
                strncpy(bufferTwo,bufferOne,recordSize);
            }

        }
    }
    fclose(handle);
}
