#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include <memory.h>
#include <sys/time.h>
#include <time.h>
#include <sys/resource.h> 
#include <time.h>
#include <unistd.h>
 
#define FAILURE_EXIT(format, ...) {                                            \
    printf(format , ##__VA_ARGS__);                       \
    exit(-1);                                                                  }

int max(int a, int b){
    return a > b ? a : b;
}

int min(int a, int b){
    return a < b ? a : b;
}

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

clock_t start;
clock_t finish;
struct tms *tmsstart;
struct tms *tmsfinish;


int main(int argc, char* argv[]){

    if(argc!=5) FAILURE_EXIT("Wrong number of arguments!\n");
    number_thr = (int) atoi(argv[1]);
    char *input = argv[2];
    char *filter_file = argv[3];
    char *output = argv[4];

    
    open_picture(input);
    open_filter(filter_file);
    
    

    pthread_t threads[number_thr];

    start = times(tmsstart);

    for(int i=0; i<number_thr; i++){
        pthread_create(&threads[i],NULL,thread_action,(void *)i);
    }

    for(int i=0; i<number_thr; i++){
        pthread_join(threads[i],NULL);
    }

    long ticks = sysconf(_SC_CLK_TCK);
    finish = times(tmsfinish);
    printf("Czas rzeczywisty: %f\n", (finish - start) / (double)ticks);

    save_picture(output);
}

int calculate_pixel(int x, int y) {
    double value = 0;
    for(int i = 0; i < C; ++i) {
        for(int j = 0; j < C; ++j) {
            int a = max(1, x - (int)ceil(C/2) + i+1);
            a = min(a,W);
            int b = max(1, y - (int)ceil(C/2) + j+1);
            b = min(b,H);
            a--;
            b--;
            value += picture[a][b] * filter[i][j];
        }
    }
    return (int) round(value);
}

int filterPixel( int x, int y){
    double newVal=0.0;
    for(int i=0;i<C;i++){
        for(int j=0;j<C;j++){
            //newVal+=picture[min(W,max(1, x-(int) ceil(C/2.0) + i+1))-1][min(H,max(1, y-(int)ceil(C/2.0)+j+1))-1] * filter[i][j];
            newVal+=picture[max(0, x-ceil(C/2.0) + i+1)][max(0, y-ceil(C/2.0)+j+1)] * filter[i][j];
            //[min(imgHeight, max(1,(x+1) - (int)ceil((double)filterSize/2.0) + (i+1))) -1]
            //[min(imgWidth , max(1,(y+1) - (int)ceil((double)filterSize/2.0) + (j+1))) -1] * filterBuffer[i][j];
        }
    }
    newVal = round(newVal);
    return (int) newVal;
}
void *thread_action(void* line){
    int line_number = (int) line;
    printf("line#%d\n",line_number);
    for(int i = line_number; i < W; i+=number_thr){
        for(int j=0; j < H; j++ ){
            
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
    printf("Width: %d, Height: %d\n",W,H);
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
            fscanf(input_file, "%i", &picture[j][i]);
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
                fprintf(output_file,"%d ",filtered[j][i]);
            }
            fprintf(output_file,"\n");
        }
        fclose(output_file);

}