#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


void generate_file(char *filePath, int recordsNumber, int recordSize);

void copy_file(char *sourceFileName, char *destFileName, int recordsNumber, int recordSize);

void sort_file(char *filePath, int recordsNumber, int recordSize);