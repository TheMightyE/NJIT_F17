#include <stdio.h>

#define SPACE "-----------------------------------\n"
int main(int argc, char **argv){
  unsigned int i=0;
  printf("argc\t%d\t |  &argc %x\n", argc, &argc);
  printf("argv\t%x |  &argv %x\n", argv, &argv);
  printf("%s", SPACE);
  
  while(*argv){
    printf("argv+%d\t%x | *argv %x | **argv=%c | &**argv=%x | *argv=%s\n", i++, argv, *argv, **argv, &**argv, *argv);
    argv++;
  }
  // argv=argv-argc;
  printf("%s%s%s", SPACE,SPACE,SPACE);
  //while(*argv){
  // printf("**argv=%c \t| *argv=%x\n", **argv, argv);
  // argv++;
  //}
  // printf("%s",SPACE);
  // argv=argv-argc;
 
  printf("argv+%d: argv=%x [ *argv=%x == &(**argv)=%x ]\n", i++, argv, *argv, &(**argv));
  return 0;
}
