#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void read_grid_from_file(int size, char *ip_file, int grid[36][36]) {
	FILE *fp;
	int i, j;
	fp = fopen(ip_file, "r");
	for (i=0; i<size; i++)
		for (j=0; j<size; j++) {
			fscanf(fp, "%d", &grid[i][j]);
	}
}

void print_grid(int size, int grid[36][36]) {
	int i, j;
	/* The segment below prints the grid in a standard format. Do not change */
	for (i=0; i<size; i++) {
		for (j=0; j<size; j++)
			printf("%d\t", grid[i][j]);
		printf("\n");
	}
}

typedef struct {
  int sudoku[36][36];
  int N, row, col;
}args;

int sud[36][36];



int check(int N, int row, int sudoku[36][36], int num, int col)
{

	for (int x = 0; x <= N-1; x++) {
		if (sudoku[x][col] == num)
            return 0;
	}

	for (int x = 0; x <= N-1; x++) {
		if (sudoku[row][x] == num)
            return 0;
	}

	int miniRow = row - row % (int)(sqrt(N)), miniCol = col - col % (int)(sqrt(N));

 	for (int i = 0; i < (int)(sqrt(N)); i++) {
		for (int j = 0; j < (int)(sqrt(N)); j++) {
			if (sudoku[i + miniRow][j +miniCol] == num)
							return 0;
		}
	}

	return 1;
}

void *counter (void *s) {

	int *size = (int *) s;
		for (int i = 0; i <= *size-1; i++) {
			for (int j = 0; j <= *size-1; j++) {
				if (sud[i][j] == 0) {
					int count = 0, n=0;
					for (int num = 1; num <= *size; num++)
				    {
							if (check(*size,i,sud,num,j)==1) {
				            count++;
										if (count>1) break;
										n = num;
								}
				    }
						if (count==1) {
							sud[i][j] = n;
						}
					}
			}
		}
}


	int solver(int row,  int sudoku[36][36], int col, int N)
	{

	    if (row == N - 1 && col == N)
	        return 1;

	    if (col == N)
	    {
	        row++;
	        col = 0;
	    }
	    if (sudoku[row][col] > 0)
	       return solver(row,sudoku,col+1,N);

	 	for (int num = 1; num <= N; num++)
	    {

		if(check(N,row,sudoku,num,col)==1)
	        {
	            sudoku[row][col] = num;
	    				if (solver(row,sudoku,col+1,N)==1)
	                return 1;
	        }
	        sudoku[row][col] = 0;
	    }
	    return 0;
	}

int main(int argc, char *argv[]) {
	int grid[36][36], N, i, j;

	if (argc != 3) {
		printf("Usage: ./sudoku.out grid_size inputfile");
		exit(-1);
	}

	N = atoi(argv[1]);


	read_grid_from_file(N, argv[2], sud);
	/* Do your thing here */


	pthread_t th[N*2];

	for (i=0; i<N*2; i++){
		pthread_create(&th[i], NULL, counter, &N );
	}


	for (i=0; i<N*2; i++){
		pthread_join(th[i],NULL);
	}

	solver(0,sud,0,N);
	print_grid(N, sud);


}
