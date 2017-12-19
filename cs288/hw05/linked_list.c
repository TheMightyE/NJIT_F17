#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define LINE_LENGTH 1024

struct clip *build_a_lst();
struct clip *append();
int find_length();
void print_lst();
void split_line();

struct clip {
  int views;
  char *user;
  char *duration;
  char *title;
  struct clip *next;
} *head;

int main(int argc, char **argv) {
  int n;
  head = build_a_lst(*(argv+1));
  print_lst(head);		/* prints the table */
  n = find_length(head);
  printf("%d clips\n",n);
  return 0;
}

struct clip *build_a_lst(char *fn) {
  FILE *fp;
  struct clip *hp;
  char *fields[4];
  char line[LINE_LENGTH];
  int cnt=0;
  hp = NULL;
  // open fn
  // while no more lines
  // read
  fp = fopen(fn,"r");
  if(fp){
    while(fgets(line,LINE_LENGTH,fp)){
      // split the line into four substrings/int and store them in a struct
      split_line(&fields,line);
      // append - add the struct at the end of the list
      hp = append(&hp,fields);
      
    }

    fclose(fp);
  }
  // return the head pointer holding the list
  return hp;
}

/* fields will have four values stored upon return */
void split_line(char **fields,char *line) {
  int i=0;
  char *token, *delim;
  delim = ",\n";
  /* 
     call strtok(line, delim);
     repeat until strtok returns NULL using strtok(NULL, delim);
 */
  token=strtok(line, delim);
  
  while(token){
    fields[i] = malloc(strlen(token) + 1);
    strcpy(fields[i],token);
    token = strtok(NULL, delim);
    i++;
  }
}

/* set four values into a clip, insert a clip at the of the list */
struct clip *append(struct clip **hp,char **four) {
  struct clip *cp,*tp;
   cp = *hp;
   // malloc tp   
   tp = malloc(sizeof(struct clip));
   tp->next = NULL;
   /* 
      malloc for four strings.
      set views using atoi(four[2])
   */
   tp->user = four[0];
   tp->duration = four[1];
   tp->views = atoi(four[2]);
   tp->title = four[3];
   //printf("user: %s\ntitle: %s\nviews: %d\ntime: %s\n\n\n", tp->user, tp->title, tp->views, tp->duration);
   // use cp to traverse the list
  if(cp){
    /*
      list not empty
      insert tp at the end of the list pointed by hp
    */
    while(cp->next)
      cp = cp->next;

    cp->next = tp;
  }else{
    //head is NULL (empty list)
    *hp = tp;
  }
  
  return *hp;
}

void print_lst(struct clip *cp) {
  /* 
     use a while loop and the statement below to print the list
     printf("%d,%s,%s,%s,%s\n",cp->views,cp->user,cp->id,cp->title,cp->time);
  */
  while (cp){
    printf("%d,%s,%s,%s\n",cp->views,cp->user,cp->title,cp->duration);
    cp = cp->next;

  }
  
}

int find_length(struct clip *cp){
  int i = 0;
  while (cp){
     i++;
     cp = cp->next;
  }
   return i;
}

/* end */
