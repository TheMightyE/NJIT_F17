/* HW6 read cafefully and follow all the instructions/comments
   this program compiles and runs as is although it's not doing much.
   DO NOT CHANGE THE SEED. Changing seed gives a different sequence of
   numbers but we want the same numbers for comparison purposes.

   FLOATING point radix sort requires some substantial changes to this code
   float n, lst[N],tmp[N];	
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdint.h>
#include <string.h>

#define BUCKETS 256
#define OK 1
#define NOK 0
#define NELM 100		/* default 100 elements */
#define N 1048576		/* 2^30 or 1 meg elements  */
//#define N 2097152
//#define N 4194304
//#define N 8388608
//#define N 16777216
//#define N 33554432

void selection_sort();
void swap();

void merge_sort();
void msort_recursive();
void merge();

void int_radix_sort();
void float_radix_sort();
void hex_radix_sort();
void count_sort();
void swap();
void self_check();

int rand(void);
void srand();
int rand_r();
void init_lst();
void print_lst();

int n, lst[N], buf[N];

int main(int argc,char **argv) {
  int l[] = {9,8,7,1};

  long int del_sec; 
  double elapsed_msec;
  struct timeval start, stop;
  if (argc>1) n = atoi(argv[1]);
  else n = NELM;
  printf("n=%d\n",n);
  init_lst(lst,n);
  //  print_lst(lst,n);
  
  if(n<=100000){
    printf("--Selection--\n");
    gettimeofday(&start, NULL); 
    selection_sort(lst,n);
    gettimeofday(&stop, NULL);
    self_check(lst,n);
    del_sec = stop.tv_sec-start.tv_sec;
    elapsed_msec = del_sec + ((stop.tv_usec - start.tv_usec)/1000000.0);
    printf("%.4f sec\n", elapsed_msec);
  }else{
    printf("--Selection skipped--\n");
  }
  printf("\n--Merge--\n");
  gettimeofday(&start, NULL);
  merge_sort(lst,buf,n);
  self_check(lst,n);
  gettimeofday(&stop, NULL);
  del_sec = stop.tv_sec-start.tv_sec;
  elapsed_msec = del_sec + ((stop.tv_usec - start.tv_usec)/1000000.0);
  printf("%.4f sec\n", elapsed_msec);
  
  
  //hex_radix_sort(lst, buf, 8, n);
  //int_radix_sort(lst,buf,n);
  
  //  float_radix_sort(lst,tmp,n);
  //  gettimeofday(&tv_e, NULL); 

  /****
    PRINT elapsed time in sec and milli secs
  ****/
  //  printf("lst: ");
  // print_lst(lst, n);
  // printf("\nbuf:--");
  //print_lst(buf, n);
  //self_check(lst,n);
  return 0;
}

void selection_sort(int lst[],int n){
  // fill here
  int smallest, i, j;
  for (i=0; i<n-1; i++){
    smallest=i;
    for (j=i+1; j<n; j++){
      if (lst[j] < lst[smallest]){
	smallest=j;
      }
    }
    swap(&lst[i], &lst[smallest]);
  }
}
void merge_sort(int lst[], int buf[], int n){
  msort_recursive(lst, buf, n);
}
int itr=0;
//use recursion
void msort_recursive(int lst[], int buf[], int n){
  // divide
  if( n > 1){
    int mid = n / 2, lenL = mid, lenR = n-mid;
    // int L[lenL], R[lenR], i;
    int *L = malloc(sizeof(int)*lenL), *R = malloc(sizeof(int)*lenR), i;
    if(L==NULL)
       fprintf(stderr, "Out of memory allocating %d bytes\n", sizeof(int)*lenL);
    if(R==NULL)
       fprintf(stderr, "Out of memory allocating %d bytes\n", sizeof(int)*lenR);
    for (i=0; i<mid; i++)
      L[i] = lst[i];
    for (i=mid; i<n; i++)
      R[i-mid] = lst[i];
    
    msort_recursive(L, buf, lenL); // divide left half
    msort_recursive(R, buf, lenR); // divide right half

    // merge
    merge(L, R, lst, lenL, lenR);
  } else{

  }
}

void merge(int left[], int right[], int lst[], int lenL, int lenR){
  //printf("Merging: ");
  //print_lst(lst,n);
  int i=0, j=0, k=0;
  while (i<lenL && j<lenR){
    if (left[i] <= right[j])
      lst[k++] = left[i++];
    else
      lst[k++] = right[j++];
  }

  // Check for leftover elements in subarrays
  while (i<lenL)
    lst[k++] = left[i++];
  while (j<lenR)
    lst[k++] = right[j++];
  
}

//fix the bucket size to 256. run 4 passes where each pass processes 8 bits
//use lst and tmp only. do not use any more memory of size n.
void hex_radix_sort(int lst[], int buf[], int group, int n) {
  int i,j,cnt[BUCKETS],map[BUCKETS], buckets = 1<<group, mask = BUCKETS-1;
  int flag,*src,*dst;
 
  // init cnt
  for (i=0; i<buckets; i++)
    cnt[i]=0;
  
  if(i%2==0){
    src=lst;
    dst=buf;
  }else{
    src=buf;
    dst=lst;
  }
  
  // count
  for (i=0; i<n; i++)
    cnt[src[i] & mask]++;

  // map
  map[0]=0;
  for (i=1; i<buckets; i++) 
    map[i] = cnt[i-1]+map[i-1];

  // move
  for (i=0; i<n; i++)
    dst[map[src[i] & mask]++] = src[i];


}

void int_radix_sort(int lst[], int buf[], int n){
  int max = 0, i;
  for (i=0; i<n; i++){
    if(lst[i]>max)
      max = lst[i];
  }

  for (i=1; max/i>0; i*=10)
    count_sort(lst, buf, 10, i, n);

}

void count_sort(int lst[], int buf[], int base, int digit, int n){
  int cnt[n], i, j;
  int pow_num;

  for (i=0; i<n; i++){
    //pow_num = pow(base,digit);
    j = (lst[i]/pow_num)%base;
    cnt[j]++;
  }

  for (i=1; i<n; i++)
    cnt[i] = cnt[i] + cnt[i-1];

  for (i=n-1; i>=0; i--){
    //j = (lst[i]/pow(base,digit))%base;
    cnt[j]--;
    buf[cnt[j]] = lst[i];
  }
}

void print_lst(int *l,int n){
  int i;
  for (i=0; i<n; i++) {
    printf("%d ",l[i]);
  }
  printf("\n");
}

void init_lst(int *l,int n){
  int i;
  //int seed = time(0) % 100;	/* seconds since 1/1/1970 */
  //printf ("seed=%d\n", seed);
  srand(1234);			/* SEED */
  for (i=0; i<n; i++) {
    l[i] = rand();
  }
}

 void swap(int *a, int *b){
   int tmp = *a;
   *a = *b;
   *b = tmp;

 }

void self_check(int *lst,int n) {
  int i,j,flag=OK,*lstp;

  lstp = lst;
  for (i=0;i<n-1;i++)
     if (lstp[i] > lstp[i+1]) { flag = NOK; break; }

  if (flag == OK) printf("sorted\n");
  else printf("NOT sorted at %d\n",i);
}

/* End of file */
