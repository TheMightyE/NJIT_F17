/* 
   Syed Khalid
   CS288 HW10
*/
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define NELMS 100000
#define MASTER 0
#define MAXPROCS 16

int dot_product();
void init_lst();
void print_lst();

int main(int argc, char **argv) {
  int i,n,vector_x[NELMS],vector_y[NELMS];
  int prod=0,tmp_prod=0,sidx,eidx,size,portion;
  int pid,nprocs;
  double stime,etime;
  MPI_Status status;
  MPI_Comm world;

  if (argv[1])
    n = atoi(argv[1]);
  else
    n = NELMS;
  if (n > NELMS) {
    printf("n=%d > N=%d\n",n,NELMS);
    exit(1);
  }
  
  init_lst(vector_x, n);
  init_lst(vector_y, n);
  
  MPI_Init(&argc, &argv);
  world = MPI_COMM_WORLD;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  
  if (nprocs > MAXPROCS){
    printf("MAXPROCS=%d You entered %d\n", MAXPROCS, nprocs);
    exit(1);
 } 
  
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  
  portion = n/nprocs;
  sidx = pid*portion;
  eidx = sidx+portion;
  
  
  stime = MPI_Wtime();
  prod = dot_product(sidx, eidx, vector_x, vector_y);  
  etime = MPI_Wtime();

  //printf("pid %d s=%d\te=%d\n", pid, sidx, eidx);
  
  if (pid == MASTER) {
    for (i=1; i<nprocs; i++){
      MPI_Recv(&tmp_prod, 1, MPI_INT, i, 123, world, &status);
      prod += tmp_prod;
      //MPI_Gather(&prod,1,MPI_INT,&tmp_prod,1,MPI_INT,MASTER,world);
    }
    // prod = tmp..
    printf("->pid=%d: final prod= %d\n",pid,prod);
    printf("->pid=%d: elapsed=%f\n",pid,etime-stime);
  }else{
    MPI_Send(&prod, 1, MPI_INT, MASTER, 123, world);
    // MPI_Scatter(vector_x,portion,MPI_INT,vector_x+sidx,portion,MPI_INT,MASTER,world);
    // MPI_Scatter(vector_y,portion,MPI_INT,vector_y+sidx,portion,MPI_INT,MASTER,world);
    printf("pid %d sent temporary product = %d\n", pid, prod);
  }
  MPI_Finalize();
}

int dot_product(int s,int e, int *vector_x, int *vector_y){
  int i,prod=0;
  for (i=s; i<e; i++){
    prod += vector_x[i] * vector_y[i];
  }
  return prod;
}

void init_lst(int *l,int n){
  int i, j=1;
  for (i=0; i<n; i++){
    if (j>10)
      j=1;
    *l++ = i;
  }
}
void print_lst(int s,int e, int *l){
  int i;
  for (i=s; i<e; i++) {
    printf("%x ",l[i]);
  }
  printf("\n");
}

// end of file
