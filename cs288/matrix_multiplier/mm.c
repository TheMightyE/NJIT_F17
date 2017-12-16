#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define N 8
#define MASTER 0
#define MAXPROCS 16

void matrix_multiplier();
void prnt_lst();
void init_lst_zero();
void init_lst();

int n;

int main(int argc, char **argv){
  int i, j, k, pid, nprocs, srow, erow, portion, tag=0;
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
  //prnt_lst(vect_y);

  MPI_Init(&argc, &argv);
  world = MPI_COMM_WORLD;
  MPI_Comm_size(world, &nprocs);
  MPI_Comm_rank(world, &pid);

  portion = n/nprocs;
  srow = pid*portion;
  erow = srow+portion;

  stime = MPI_Wtime();
  matrix_multiplier(vect_x, vect_y, vect_z, srow, erow);
  etime = MPI_Wtime();
  
  printf("pid %d: Elapsed=%f\n",pid,etime-stime);
  int nelms = portion*n;
  
  if (pid == MASTER){
    //MPI_Gather(&vect_z[nelms*portion], nelms, MPI_INT, vect_z, nelms, MPI_INT, MASTER, world);
    for (i=1; i<nprocs; i++){
      MPI_Recv(vect_z+i*portion, nelms, MPI_INT, i, tag, world, &status);
      
    }
    //printf("pid 0: Elapsed=%f\n",etime-stime);
    //prnt_lst(vect_z);
  }else{
    // MPI_Scatter(vect_x, nelms, MPI_INT, vect_x+nelms*pid, nelms, MPI_INT, MASTER, world);
    MPI_Send(vect_z+srow, nelms, MPI_INT, MASTER, tag, world);
  }
  MPI_Finalize();
  return 0;
}

void matrix_multiplier(int x[n][n], int y[n][n], int z[n][n], int srow, int erow){
  int i=0, j=0, k=0;
  for (i=srow; i<erow; i++){
    for (j=0; j<n; j++){ 
      for (k=0; k<n; k++){
	z[i][j] = z[i][j]+x[i][k]*y[k][j];
      }
    }
  }

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
      printf("%7d ", m[i][j]);
    }
    printf("\n");
  }
}
