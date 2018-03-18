#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void generate_file(char *filePath, int recordsNumber, int recordSize){
    int handle = open(filePath, O_WRONLY | O_CREAT);
    int rand_handle = open("/dev/urandom", O_RDONLY);
    unsigned char *buffer = calloc(recordSize, sizeof(char));
    if (handle && rand_handle){
        for (int i = 0; i < recordsNumber; i++){
            
            if(!read(rand_handle, buffer,recordSize)){
            printf("Reading from /dev/urandom failed, shutting down.");
            close(handle);
            close(rand_handle);
            exit(1);
            }

            
            for (int j = 0; j < recordSize; j++){
                buffer[j] = buffer[j] % 52;
                if (buffer[j] < 26) buffer[j] = buffer[j] + 65;
                else buffer[j] = buffer[j] + 71;
            }
            if (!write(handle, buffer, recordSize)){
                printf("Writing to %s failed, shutting down.", filePath);
                close(handle);
                close(rand_handle);
                exit(1);
            }
        }
    }
    close(handle);
    close(rand_handle);
}


void copy_file(char *sourceFileName, char *destFileName, int recordsNumber, int bufferSize){
    int sourceFile = open(sourceFileName, O_RDONLY);
    int destFile = open(destFileName, O_WRONLY|O_CREAT);
    unsigned char* buffer = calloc(bufferSize,sizeof(char));
    if(sourceFile && destFile){
        for(int i=0; i < recordsNumber; i++){
            if(!read(sourceFile,buffer,bufferSize)){
                printf("Reading from %s failed, shutting down.",sourceFileName);
                close(sourceFile);
                close(destFile);
                exit(1);
            }
            if(!write(destFile,buffer,bufferSize)){
                printf("Writing to %s failed, shutting down.",destFileName);
                close(sourceFile);
                close(destFile);
                exit(1);
            }
        }
    }
    close(sourceFile);
    close(destFile);
}

void sort_file(char *filePath, int recordsNumber, int recordSize){
    int handle = open(filePath, O_RDWR);
    char* bufferOne = calloc(recordSize,sizeof(char));
    char* bufferTwo = calloc(recordSize,sizeof(char));
    if(handle){
        for(int i=1; i < recordsNumber; i++){
            lseek(handle,i*recordSize,SEEK_SET);
            read(handle,bufferOne,recordSize);
            int j=0;
            while(1){
                lseek(handle,j*recordSize,SEEK_SET);
                read(handle,bufferTwo,recordSize);
                if(j>=i || bufferOne[0]<bufferTwo[0]){
                    break;
                } 
                j++;
            }

            lseek(handle,-recordSize,SEEK_CUR);
            write(handle,bufferOne,recordSize);
            for(int k = j+1; k < i+1; k++){
                lseek(handle,k*recordSize,SEEK_SET);
                read(handle,bufferOne,recordSize);
                lseek(handle,-recordSize,SEEK_CUR);
                write(handle,bufferTwo,recordSize);
                strncpy(bufferTwo,bufferOne,recordSize);
            }

        }
    }
    close(handle);
}