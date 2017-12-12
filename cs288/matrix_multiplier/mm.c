#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define N 8
#define MASTER 0
#define MAXPROCS 16

int matrix_multiplier();
void prnt_lst();
void init_lst_zero();
void init_lst();

int n;

int main(int argc, char **argv){
  int i, j, k, pid, nprocs, srow, erow, portion;
  double stime, etime;
  MPI_Status status;
  MPI_Comm world;

  
  if (argv[1])
    n = atoi(argv[1]);
  else
    n = N;
  
  int  vect_x[n][n], vect_y[n][n], vect_z[n][n];
  init_lst(vect_x);
  init_lst(vect_y);
  init_lst_zero(vect_z, n);
  prnt_lst(vect_y);

  MPI_Init(&argc, &argv);
  world = MPI_COMM_WORLD;
  MPI_Comm_size(world, &nprocs);
  MPI_Comm_rank(world, &pid);

  portion = n/nprocs;
  srow = pid*portion;
  erow = srow+portion;

  printf("pid=%d\tsrow=%d\terow=%d\n", pid, srow, erow);

  if (pid == MASTER){
    for (i=1; i<nprocs; i++)
      MPI_Recv(&vect_z[i*portion], portion, MPI_INT, i, 123, world, &status);
  }else{
    // MPI_Send();
  }

  return 0;
}

void init_lst_zero(int m[n][n], int n){
  int i, j;
  for (i=0; i<n; i++){
    for(j=0; j<n; j++){
      m[i][j] = 0;
    }
  }
}

void init_lst(int m[n][n]){
  int i, j, k=0;
  for (i=0; i<n; i++){
    for(j=0; j<n; j++){
      m[i][j] = k++;
    }
  }
}

void prnt_lst(int m[n][n]){
  int i, j;
  for (i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%2d ", m[i][j]);
    }
    printf("\n");
  }
}
