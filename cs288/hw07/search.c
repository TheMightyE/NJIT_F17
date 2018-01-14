/******************************************************************************
 CS288 HW7 three search strategies: depth, breadth, and intelligent
 command example: command 16 numbers and search strategy

 fifteen 1 2 3 4 5 6 7 8 9 10 11 12 13 14 0 15 {dfs|bfs|astar}
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

#define BF 4			/* Branching factor of the search tree */
#define N 4
#define NxN 16
#define DFS 1                   /* depth first search */
#define BFS 2                   /* breadth first search */
#define BEST 3                  /* best first search */
#define BB 4                    /* branch and bound */
#define ASTAR 5                 /* A* search */
#define UNKNOWN 9		/* unknown search strategy */

#define MAX_PATH 1000

#define DN 0			/* for finding path */
#define RT 1
#define UP 2
#define LT 3
#define UK 9

#define FVAL 0			/* f=g+h, keep it simple for now */
#define GVAL 1
#define HVAL 2
#define PATH 3			/* array index, how it got to this state */

#define TRUE 1
#define FALSE 0

int level,strategy;

int nodes_same(),str_compare(),count(),find_h();
void swap(),exit_proc(),print_a_node(),print_nodes();
int toggle_dir(), solvable();
void find_path(),print_path(),find_parent();
int path[MAX_PATH],path_buf[MAX_PATH];
void prep_dir_to_str(),dir_to_str();
char *dir_to_sdir[4],strategy_s[10]; /* four directions */

struct node {
  int board[N+1][N];
  struct node *next;
};

int g=0;
struct node *start,*goal;
struct node *initialize(),*expand(),*merge(),*filter(),*move(),*prepend(),*append(),*filter2();
struct node *insert_node(),*check_list(),*goal_found();

int main(int argc,char **argv) {
  int iter,cnt=0,total=1,ocnt=0,ccnt=0;
  int perm;		/* perm=odd=no solution, perm=even=solvable */
  struct node *cp,*open,*closed,*succ,*tp;
  long int del_sec; 
  double elapsed_sec;
  struct timeval st, et;
  
  open=closed=succ=NULL;
  start=initialize(argc,argv);	/* init initial and goal states */
  perm=solvable(start);         /* check if solvable permutation */
  if (perm & 1){
    printf("The initial state of the board is unsolvable\n");
    
    printf("perm:%d\n",perm);
    return 1;
  }

  open=start; 
  iter=0;
  
  gettimeofday(&st, NULL);
  while (open) {
    /*if (iter==10){
      print_nodes(open, "open");
      break;
      }*/
    cp=open; open=open->next; cp->next=NULL; /* get the first node from open */
    succ = expand(cp);
    succ = filter(succ,open);		     /* New succ list */
    succ = filter(succ,closed);		     /* New succ list */
    cnt=count(succ);
    total=total+cnt;
    
    if (succ) open=merge(succ,open,strategy); // New open list
    closed=append(cp,closed);		      // New closed
    if ((cp=goal_found(succ,goal))){
      printf("\n\nGoal found in succ\n");
      print_a_node(cp);
      break;
    }
    iter++;
  }
  gettimeofday(&et, NULL);
  
  del_sec = et.tv_sec-st.tv_sec;
  elapsed_sec = del_sec + ((et.tv_usec - st.tv_usec)/1000000.0);
  if (elapsed_sec > 60)
    printf("Took: %.4f min\n", elapsed_sec/60);
  else
    printf("Took: %.4f secs\n", elapsed_sec);

  ocnt=count(open);
  ccnt=count(closed);
  printf("%d: open=%d + clsd=%d = total nodes=%d\n",iter+1,ocnt,ccnt,total);
  printf("%s strategy: %d iterations\n",strategy_s,iter+1);
  return 0;
} /* end of main */

int toggle_dir(int dir){
  int opp_dir;
  
  if (dir == UP)
    opp_dir = DN;
  else if (dir == DN)
    opp_dir = UP;
  else if (dir == LT)
    opp_dir = RT;
  else if (dir == RT)
    opp_dir = LT;
  else
    opp_dir = UK;
  
  // return opp direction
  return opp_dir;
}

void print_path(int n,int *path){
  int i,p;
  //  for (i=0;i<n;i++) path[i] = path_buf[path_cnt-i-1];
  //  for (i=0;i<path_cnt;i++) printf("%d ",path[i]); printf("\n");
  printf("entering print_path: n=%d\n",n);

  ////  for (i=n-1;i>=0;i--) printf("%d ",*(path+i)); printf("\n");
  for (i=n-1;i>=0;i--) {
    p = *(path+i);
    if (i>0) printf("%s -> ",dir_to_sdir[p]);
    else printf("%s",dir_to_sdir[p]);    
  }
  printf("\n");
  //  printf("exiting print_path\n");
}

//char **dir_to_sdir={"DN","RT","UP","LT"}; //[4];
void prep_dir_to_str(){
  //...
}

void find_path(struct node *cp,struct node *opnp,struct node *cldp){
  int i,j,dir,opp_dir;
  char *sdir,*opp_sdir;
  int path_cnt=0,p;
  struct node *tp;
  //start from the very last node that matches the goal state because
  //that has the direction info
  //toggle the direction, get the parent
  //look for that parent in closed
  //repeat until you find the initial state
}

void find_parent(struct node *cp,int prev_dir){
  int i,j,k,cnt,row=0,col=j;
  
}

// Expand: generate successors of the current node
struct node *expand(struct node *cp) {
  int i,j,k,cnt,row=0,col=j;
  struct node *succ,*tp;
  //succ=malloc(sizeof(struct node));
  succ=NULL;
  tp=malloc(sizeof(struct node));
  tp->next=NULL;
  g++;
  /* check where 0 is. [i,j] after the for loop represents the location of that 0 */
  for(i=0; i < N; i++){
    for(j=0; j < N; j++)
      if (cp->board[i][j]==0) break;
    if (j<N) break;		/* found it */
  }

  if((i+1) < N){		/* DOWN */
    tp = move(cp,i,j,i+1,j,DN);
    succ = append(tp,succ);
    
  }
  if((j+1) < N){                /* RIGHT */
    tp = move(cp,i,j,i,j+1,RT);
    succ = append(tp,succ);
  }
  if((i-1) < N){		/* UP */
    tp = move(cp,i,j,i-1,j,UP);
    succ = append(tp,succ);
  }
  if((j-1) < N){		/* LEFT */
    tp = move(cp,i,j,i,j-1,LT);
    succ = append(tp,succ);
  }
  return succ;
}

/* attach in the beginning */
struct node *prepend(struct node *tp,struct node *sp) {
  //.....
  struct node *tmp = sp;
  sp = tp;
  tp->next = tmp;
  return sp;
}

/* attach at the end */
struct node *append(struct node *tp,struct node *sp) {
  struct node *cp;
  cp = sp;
  /* list is empty case*/
  if (!cp){
    sp=tp;
  }else{
    while(cp->next)
      cp = cp->next;
    cp->next = tp;
  }
  return sp;
}

void swap(struct node *cp,int i,int j,int k,int l){
  int tmp;
  tmp = cp->board[i][j];
  cp->board[i][j] = cp->board[k][l];
  cp->board[k][l] = tmp;
}

// Move from a,b to x,y
struct node *move(struct node *cp,int a,int b,int x,int y,int dir) {
  struct node *newp;
  int i,j,k,l,distance,m,n,tmp=0;
  //malloc
  newp = malloc(sizeof(struct node));
  newp->next = NULL;
  
  // copy from cp
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      newp->board[i][j] = cp->board[i][j];
    }
  }
  // swap two vals: from a,b > to x,y
  swap(newp, a, b, x, y);
  // compute f,g,h
  // compute h with the distance formula
  int h = find_h(newp,goal);
  
  newp->board[N][GVAL] = g; // increment g
  newp->board[N][HVAL] = h; //set computed h value
  newp->board[N][FVAL] = newp->board[N][GVAL] + newp->board[N][HVAL];
  // insert the direction that resulted in this node, used for printing path
  newp->board[N][PATH] = dir;
  //print_a_node(newp);
  //printf("h: %d\n\n", tmp);
   
  return newp;
}

struct node *goal_found(struct node *cp,struct node *gp){
  int flg=FALSE;
  // check if succ list has goal
  while (cp){
    if(nodes_same(cp,gp)){
      return cp;
    }
    cp=cp->next;

  }
  // if found, return that for finding path else return NULL
  return NULL;
}

int count(struct node *cp) {
  int cnt=0;
  while (cp){
    cp = cp->next;
    cnt++;
  }
  //return the number of nodes on the list
  return cnt;
}

struct node *merge(struct node *succ,struct node *open,int flg) {
  struct node *csucc,*copen;
  csucc = succ;
 
  if (flg==DFS) { /* attach in the front: succ -> ... -> open */
    while (csucc){
      open = prepend(csucc,open);
      csucc = csucc->next;

    }
    
  }else if (flg==BFS) {	  /* attach at the end: open -> ... -> succ */
    while (csucc){
      open = append(csucc,open);
      csucc = csucc->next;
    }
    
  }else if (flg==BEST) {	/* Best first: sort on h value */
    // merge on h value
    while (csucc){
      open = insert_node(csucc, open, HVAL);
      csucc = csucc->next;
      
    }
  }else{			/* A* search: sort on f=g+h value */
    // merge on f value
    while (csucc){
      open = insert_node(csucc, open, FVAL);
      csucc = csucc->next;
      
    }
  }
  return open;
}


/* insert succ into open in ascending order of x value, where x is an array 
   index: 0=f,1=g,h=2 of board[N][x]
*/
struct node *insert_node(struct node *succ_node,struct node *open,int x) {
  int i, j;
  struct node *copen, *csucc, *newp;
  newp = malloc(sizeof(struct node));
  newp->next = NULL;
  /* Copy csucc to newp and insert it later */
  for (i=0; i<N+1; i++){
    for (j=0; j<N; j++){
      newp->board[i][j] = succ_node->board[i][j];
    }
  }
  /* Special case: 'open' list is empty */
  if (!open || newp->board[N][x] <= open->board[N][x]){
    //printf("newp: %d | copen->next: %d\n", newp->board[N][x], open->board[N][x]);
    newp->next = open;
    open = newp;
    //printf("inserted in beg hval:%d!\n",newp->board[N][x]);
  }else{
    /* Insert 'newp' somewhere in middle of list pointed by 'open' */
    copen = open;
    while (copen->next && copen->next->board[N][x] <=  newp->board[N][x]){
      copen = copen->next;
    }
    newp->next = copen->next;
    copen->next = newp;
     
    //printf("inserted in middle!\n");
  }
  
  return open;
}


int find_h(struct node *current,struct node *goalp) {
  int h=0,i,j,k,l,done,distance,a,b,tmp[N*N];
  struct node *tp;
  tp = malloc(sizeof(struct node));
  tp->next=NULL;
  /* Compute the Manhattan Distance */
  for (i=0; i<N; i++){ //x1
    for (j=0; j<N; j++){//y1
      for (k=0; k<N; k++){//x2
	for (l=0; l<N; l++){//y2
	  if (current->board[i][j] == goalp->board[k][l]){
	    distance = abs(i-k) + abs(j-l);
	    tp->board[i][j] = distance;
	    
	  }
	}
      }
    }
  }
  /* Add up all of the h values that were computed for each digit on the board */
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      h+=tp->board[i][j];
    }
  }
  
  return h;
}

/* a=b=x[N][N] */
int nodes_same(struct node *xp,struct node *yp) {
  int i,j,flg=TRUE;
  
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      if (xp->board[i][j] != yp->board[i][j]){
	return FALSE;
      }
    }
  }
  
  return flg;
}
struct node *filter2(struct node *succ,struct node *hp){
  int i,j,flg=0;
  struct node *cp, *csucc, *new_succ, *tp;
  tp = (struct node *)malloc(sizeof(struct node));
  tp->next = NULL;
  
  while (csucc){
    cp = hp;
    printf("in suc loop\n");
    while (cp){
    printf("in open loop\n");
      if (nodes_same(csucc, cp)){
	printf("nodes same\n");
	flg=1;
	break;
      }
      else
	cp = cp->next;
    }
    printf("flg:%d\n",flg);
    
    if (flg){
    printf("copying\n");
      for (i=0; i<N+1; i++){
	for (j=0; j<N; j++){
	  tp->board[i][j] = csucc->board[i][j];
	}
      }
      printf("appending to new_succ\n");
      new_succ = append(tp, new_succ);
      printf("appended to new_succ\n");
      
    }
    printf("next\n");

    csucc = csucc->next;    
  }
  print_nodes(new_succ, "ns");
  
  return new_succ;
}
/******************************************************************************
  Check succ against open and closed. Remove those succ nodes that are in open or closed.
******************************************************************************/
struct node *filter(struct node *succ,struct node *hp){
  struct node *lsp,*rsp,*cp;	/* lsp=left succ p, rsp=right succ p */
  struct node *tp;		/* temp ptr */
  lsp=rsp=succ;
  
  while(rsp){
    cp=hp;
    while(cp && !nodes_same(rsp, cp)){
      // lsp=rsp;
      cp=cp->next;
      
    }
    if(cp){
      /* same, so delete cp */
      if (lsp==rsp){ // first one is same
	succ = succ->next;
	lsp=rsp=succ;
      }else{
	lsp->next=rsp->next;
	rsp=rsp->next;
	
      }
    }else{
      /*save cp*/
      lsp=rsp;
      rsp=rsp->next;
    }

  }
  return succ;
}

void print_nodes(struct node *cp,char name[20]) {
  int i;
  printf("%s:\n",name);
  while (cp) { print_a_node(cp->board); cp=cp->next; }
}

void print_a_node(struct node *np) {
  int i,j;
  for (i=0;i<N+1;i++) {
    for (j=0;j<N;j++) printf("%3d ",np->board[i][j]);
    printf("\n");
  }
  printf("\n");
}

//cnt=odd -> no solution, cnt=even=solvable
int solvable(struct node *cp) {
  int i,j,k=0,lst[N*N],cnt=0,total=0;
  //flatten the board into flat lst and work with that
  for (i=0; i<N; i++){
    for (j=0; j<N; j++){
      lst[k++] = cp->board[i][j];
    }
  }
  
  for (i=0; i<N*N; i++){
    cnt=0;
    for (j=0; j<N*N; j++){
      if (lst[i] > lst[j]) cnt++;
    }
    total+=cnt;
  }
  return total;	/* return the number we discussed in class today 3/31/2015*/
}

/* fif 0 1 2 4 5 6 3 8 9 10 7 12 13 14 11 15 astar */
struct node *initialize(int argc, char **argv){
  int i,j,k,npe,n,idx,gidx,inv;
   struct node *tp;

   tp=(struct node *) malloc(sizeof(struct node));
   idx = 1;
   for (j=0;j<N;j++)
     for (k=0;k<N;k++) tp->board[j][k]=atoi(argv[idx++]);
   for (k=0;k<N;k++) tp->board[N][k]=0;	/* set f,g,h of initial state to 0 */
   tp->next=NULL;
   start=tp;

   printf("init state: \n");
   print_a_node(start);

   tp=(struct node *) malloc(sizeof(struct node));
   gidx = 1;
   for (j=0;j<N;j++)
     for (k=0;k<N;k++) tp->board[j][k] = gidx++;
   tp->board[N-1][N-1] = 0;		/* empty tile=0 */
   for (k=0;k<N;k++) tp->board[N][k]=0;	/* set f,g,h of goal state to 0 */
   tp->next=NULL;
   goal=tp;

   printf("goal state: \n");
   print_a_node(goal);

   strcpy(strategy_s,argv[idx]);
   if (strcmp(strategy_s,"dfs")==0) strategy=DFS;
   else if (strcmp(strategy_s,"bfs")==0) strategy = BFS;
   else if (strcmp(strategy_s,"best")==0) strategy=BEST;
   else if (strcmp(strategy_s,"bb")==0) strategy=BB;
   else if (strcmp(strategy_s,"astar")==0) strategy=ASTAR;
   else strategy=UNKNOWN;
   printf("strategy=%s\n",strategy_s);

   return start;
}

void exit_proc(char *msg){
   printf("Error: %s\n",msg);
   exit(1);
}

/*****************************************************************************
 End of file: fif.c. Fifteen Puzzle, Sequential A* 1 processor version.
*****************************************************************************/
