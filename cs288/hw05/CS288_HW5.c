#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define LINE_LENGTH 512

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
  //n = find_length(head);
  printf("%d clips\n",n);
  print_lst(head);		/* prints the table */
  return 0;
}

struct clip *build_a_lst(char *fn) {
  FILE *fp;
  struct clip *hp;
  char *fields[4];
  char line[LINE_LENGTH];
  int cnt=0;
  hp=NULL;
  fp=fopen(fn,"r");
  if(fp){
    while(fgets(line,LINE_LENGTH,fp)){
      split_line(&fields,line);
      hp = append(&hp,fields);
    }
    
    fclose(fp);
  }


  // open fn
  // while no more lines
  // read a line  
  // split the line into four substrings/int and store them in a struct
  // append - add the struct at the end of the list
  // return the head pointer holding the list

  return hp;
}

int find_length(struct clip *cp){
  int count = 0;

  while(cp != NULL){
    count++;
    cp = cp->next;
  }
  
  return count;
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
  while(token){
    fields[i] = malloc(strlen(token) + 1);
    strcpy(fields[i], token);
    token = strtok(NULL, delim);
    i++;
  }
}

/* set four values into a clip, insert a clip at the of the list */
struct clip *append(struct clip **hp,char **four) {
  struct clip *cp,*tp;
  cp = *hp;
  
  tp = malloc(sizeof(struct clip));
  tp->next = NULL;
  
  tp->user = four[0];
  tp->duration = four[1];
  tp->views = atoi(four[2]);
  tp->title = four[3];
  
  if(cp){
    while(cp->next){
      cp=cp->next;
    }
    cp->next = tp;  
  }
  else{
    *hp = tp;
  }
  
  /* 
     malloc tp
     set views using atoi(*five)
     malloc for four strings.
     strcpy four strings to tp
     insert tp at the end of the list pointed by hp
     use cp to traverse the list
 */

  return *hp;
}

void print_lst(struct clip *cp) {
  /* 
     use a while loop and the statement below to print the list
     printf("%d,%s,%s,%s,%s\n",cp->views,cp->user,cp->id,cp->title,cp->time);
  */
  while(cp){
    cp = cp->next;
    printf("%s\n", cp->user);
  }
}

/* end */
