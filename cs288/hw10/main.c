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

#define NELMS 16
#define MASTER 0
#define MAXPROCS 16

int dot_product();
void init_lst();
void print_lst();

int main(int argc, char **argv) {
  int i,n,vector_x[NELMS],vector_y[NELMS],*tmp_prods;
  int prod=0,sidx,eidx,size,portion;
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
  //printf("pid %d\n",pid);
  MPI_Init(&argc, &argv);
  world = MPI_COMM_WORLD;
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  
  if(pid==MASTER){
    init_lst(vector_x, n);
    init_lst(vector_y, n);
  }
  
  portion = n/nprocs;
  sidx = pid*portion;
  eidx = sidx+portion;
  
  MPI_Scatter(vector_x,portion,MPI_INT,&vector_x+sidx,portion,MPI_INT,MASTER,world);
  MPI_Scatter(vector_y,portion,MPI_INT,&vector_y+sidx,portion,MPI_INT,MASTER,world);
  
  printf("pid:%d\n",pid);
  print_lst(sidx,eidx,vector_x);
  
  stime = MPI_Wtime();
  prod = dot_product(sidx, eidx, vector_x, vector_y);
  
  printf("pid %d sent temporary product = %d\n", pid, prod);
  //printf("pid %d s=%d\te=%d\n", pid, sidx, eidx);
  if (pid == MASTER){
    tmp_prods = (int *)malloc(sizeof(int)*nprocs);
  }
  
  MPI_Gather(&tmp_prods,1,MPI_INT,&prod,1,MPI_INT,MASTER,world);
  
  if (pid == MASTER) {
    for (i=0; i<nprocs;i++){
      prod += tmp_prods[i];
    }
    
    etime = MPI_Wtime();
    printf("->pid=%d: final prod= %d\n",pid,prod);
    printf("->pid=%d: elapsed=%f\n",pid,etime-stime);
  }
  //printf("pid %d sent temporary product = %d\n", pid, prod);
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
    *l++ = 1;
  }
}
void print_lst(int s,int e, int *l){
  int i;
  for (i=s; i<e; i++) {
    printf("%2d ",l[i]);
  }
  printf("\n");
}

// end of file
