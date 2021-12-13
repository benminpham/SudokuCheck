#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS       27
#define PUZZLE_SIZE             9

int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] =
{
    {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
    {-1,5,3,4,6,7,8,9,1,2},
	{-1,6,7,2,1,9,5,3,4,8},
	{-1,1,9,8,3,4,2,5,6,7},
	{-1,8,5,9,7,6,1,4,2,3},
	{-1,4,2,6,8,5,3,7,9,1},
	{-1,7,1,3,9,2,4,8,5,6},
	{-1,9,6,1,5,3,7,2,8,4},
	{-1,2,8,7,4,1,9,6,3,5},
	{-1,3,4,5,2,8,6,1,7,9}};

int status_map[NUMBER_OF_THREADS] = {0};
typedef struct
{
	int thread_number;
	int row;
	int column;
} parameters;

void* foo(void* p){
	parameters *p1 = (parameters*) p;
	int row = p1->row;
	int col = p1->column;
	int threadNum = p1->thread_number;
	int arr[PUZZLE_SIZE] = {0};
	int i, j, k = 0;
	for(i = row; i < row + 3; i++){
		for(j = col; j < col + 3; j++){
			int num = puzzle[i][j];
			if(num < 1 || num > 9){
				status_map[threadNum] = 0;
				pthread_exit(NULL);
			}
            arr[k] = num;
			k++;
		}
	}
	for(i = 0; i < PUZZLE_SIZE; i++){
		for(j = 0; j < PUZZLE_SIZE+1; j++){
			if(i != j){
				if(arr[i] == arr[j]){
					status_map[threadNum] = 0;
					pthread_exit(NULL);
				}
			}
		}
	}
    status_map[threadNum - 1] = 1;
	pthread_exit(&p);
}
void* valRow(void* p){
	parameters *p1 = (parameters*) p;
	int row = p1->row;
	int col = p1->column;
	int threadNum = p1->thread_number;
	if(row > PUZZLE_SIZE+1 || col > PUZZLE_SIZE+1 || row < 1 || col < 1  ){
		status_map[threadNum] = 0;
		pthread_exit(NULL);
	}
    int arr[PUZZLE_SIZE] = {0};
	int i;
	for(i = 1; i < PUZZLE_SIZE+1; i++){
		int num = puzzle[col][i];
		if(num < 1 || num > 9){
			status_map[threadNum] = 0;
			pthread_exit(NULL);
		}
		arr[i-1] = num;
	}
	int j;
	for(i = 0; i < PUZZLE_SIZE; i++){
		for(j = 0; j < PUZZLE_SIZE+1; j++){
			if(i != j){
				if(arr[i] == arr[j]){
					status_map[threadNum] = 0;
					pthread_exit(NULL);
				}
			}
		}
	}
    status_map[threadNum - 1] = 1;
	pthread_exit(&p);
}
void* valCol(void* p){
	parameters *p1 = (parameters*) p;
	int row = p1->row;
	int col = p1->column;
	int threadNum = p1->thread_number;
	if(row > PUZZLE_SIZE || col > PUZZLE_SIZE || row < 1 || col < 1 ){
		status_map[threadNum] = 0;
		pthread_exit(NULL);
	}
	int arr[PUZZLE_SIZE] = {0};
	int i;
	for(i = 1; i < PUZZLE_SIZE+1; i++){
		int num = puzzle[i][row];
		if(num < 1 || num > 9){
			status_map[threadNum] = 0;
			pthread_exit(NULL);
		}
		arr[i-1] = num;
	}
	int j;
	for(i = 0; i < PUZZLE_SIZE; i++){
		for(j = 0; j < PUZZLE_SIZE+1; j++){
			if(i != j){
				if(arr[i] == arr[j]){
					status_map[threadNum] = 0;
					pthread_exit(NULL);
				}
			}
		}
	}
	status_map[threadNum - 1] = 1;
	pthread_exit(&p);
}

int main(int argc, char *argv[])
{
	char filename[100];
	printf("Please enter your filename:\n");
	scanf("%s",filename);
	
    FILE *myFile;
	myFile = fopen(filename, "r");
	int i,j;
	int threadIndex = 0;
	if (myFile == NULL){
		printf("Error Reading File\n");
		exit (0);
	}
	for (i = 1; i < PUZZLE_SIZE + 1; i++){
		for (j =1; j < PUZZLE_SIZE + 1; j++){
			fscanf(myFile, "%d,", &puzzle[i][j] );
        }
	}
	pthread_t workers[NUMBER_OF_THREADS];
	for(i = 0; i < PUZZLE_SIZE + 1; i++){
		for(j = 1; j < PUZZLE_SIZE + 1; j++){
			if(i == 1){
				parameters *rowData = (parameters *) malloc(sizeof(parameters));
				rowData ->row = i +1;
				rowData ->column = j;
				rowData ->thread_number = threadIndex + 1;
				pthread_create(&workers[threadIndex], NULL, valRow, rowData);
				threadIndex++;
			}
			if(j == 1){
				parameters *columnData = (parameters *) malloc(sizeof(parameters));
				columnData ->row = i +1;
				columnData ->column = j;
				columnData ->thread_number = threadIndex + 1;
				pthread_create(&workers[threadIndex], NULL, valCol, columnData);
				threadIndex++;
			}
			if(i%3 == 1 && j%3 == 1 ){
				parameters *threeData = (parameters *) malloc(sizeof(parameters));
				threeData ->row = i;
				threeData ->column = j;
				threeData ->thread_number = threadIndex + 1;
				pthread_create(&workers[threadIndex], NULL, foo, threeData);
				threadIndex++;
			}
		}
	}
	for(i = 0; i < NUMBER_OF_THREADS ; i++){
		pthread_join(workers[i], NULL);
	}
	int test = 0 ;
	for(i = 0; i < NUMBER_OF_THREADS; i++){
		if(status_map[i] == 0){
			printf("Sudoku is invalid at thread: %d| ", i+1);
			test = 1;
		}
    }
	if(test == 1){
		return 0;
    }
	printf("Sudoku solution is valid!");
	return 0;
}
