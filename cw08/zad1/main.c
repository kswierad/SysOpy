#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h> 
 
#define FAILURE_EXIT(format, ...) {                                            \
    printf(format , ##__VA_ARGS__);                       \
    exit(-1);                                                                  }

#define max(a, b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
#define min(c, d) \
   ({ __typeof__ (c) _c = (c); \
       __typeof__ (d) _d = (d); \
_c > _d ? _d : _c; })

int calculate_pixel(int x, int y);
void *thread_action(void* line);
void open_picture(char* file);
void open_filter(char* file);
void save_picture(char* file);

int **picture = NULL;
int **filtered = NULL;
double **filter;
int number_thr;

int C;                  
int W;
int H;



int main(int argc, char* argv[]){

    if(argc!=5) FAILURE_EXIT("Wrong number of arguments!\n");
    printf("test\n");
    number_thr = (int) atoi(argv[1]);
    char *input = argv[2];
    char *filter_file = argv[3];
    char *output = argv[4];

    
    open_picture(input);
    printf("test\n");
    open_filter(filter_file);
    printf("test\n");
    
    

    pthread_t threads[number_thr];


    for(int i=0; i<number_thr; i++){
        pthread_create(&threads[i],NULL,thread_action,(void *)i);
    }
    printf("test\n");
    for(int i=0; i<number_thr; i++){
        pthread_join(threads[i],NULL);
    }

    save_picture(output);
    printf("test\n");
}

int calculate_pixel(int x, int y) {
    int i,j;
    double value = 0;
    for(i = 0; i < C; ++i) {
        for(j = 0; j < C; ++j) {
            int a = max(1, x - (int)ceil(C/2) + i+1);
            a = min(a,H);
            int b = max(1, y - (int)ceil(C/2) + j+1);
            b = min(b,W);
            a--;
            b--;
            value += picture[a][b] * filter[i][j];
        }
    }
    return (int) round(value);
}

void *thread_action(void* line){
    int line_number = (int) line;
    for(int i=line_number*W/number_thr; i < (line_number+1)*W/number_thr; i++ ){
        for(int j = 0; j < H; j++){
            filtered[i][j] = calculate_pixel(i,j);
        }
    }
    pthread_exit(NULL);
}

void open_picture(char* file){
    FILE *input_file;
    input_file = fopen(file,"r");
    if(input_file == NULL) FAILURE_EXIT("Couldn't open input file! \n");
    int maxval;
    fscanf(input_file, "P2 %d %d %d", &W, &H, &maxval);
    printf("test\n");
    if (maxval != 255)
    FAILURE_EXIT("Wrong file format: Wrong max value!");

    picture = malloc(sizeof(int*)*W);
    for(int i = 0; i < W; i++){
        picture[i] = malloc(sizeof(int)*H);
    }

    filtered = malloc(sizeof(int*) * W );
    for(int i = 0; i < W; i++) {
        filtered[i] = malloc(sizeof(int) * H);
    }

    for(int i = 0; i < H; i++) {
        for(int j = 0; j < W; j++){
            fscanf(input_file, "%i", &picture[i][j]);
        }
    }

    fclose(input_file);


}


void open_filter(char* file){
    FILE *filter_file;
    filter_file = fopen(file,"r");

    fscanf(filter_file,"%d",&C);
    filter = malloc(sizeof(double*)*C);
    for(int i = 0; i < C; i++){
        filter[i] = malloc(sizeof(double)*C);
    }

    for(int i = 0; i < C; i++){
        for(int j = 0; j < C; j++){
            fscanf(filter_file, "%lf",&filter[i][j]);
        }
    }

}

void save_picture(char* file){
        FILE *output_file =fopen(file,"w");
        fprintf(output_file,"P2\n%d %d\n255\n",W,H);
        for(int i = 0; i < H; i++){
            for(int j = 0; j < W; j++){
                fprintf(output_file,"%d ",filtered[i][j]);
            }
            fprintf(output_file,"\n");
        }
        fclose(output_file);

}