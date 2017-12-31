#include <stdio.h>
#include <stdlib.h>

#define ROWS 25
#define COLS 40
#define CHARS ROWS*COLS

/*put in dot h file*/
enum colour{black, red, green, yellow, blue, magenta, cyan, white};
enum graphmod{alpnum, contig, seprt};
enum height{sgl, dbl};

struct byte {
  int data;
  colour bckgrcol;
  colour frgrcol;
  graphmod graphics;
  height height;
}

void print_array(int* y);

int main(void)
{
  FILE* fp;
  int x, i;
  int *y;
  y = (int*) calloc(CHARS,sizeof(int));
  i = 0;
  x = 0;
  fp = fopen("test.m7","rb");
  while(fread(&x,1,1,fp)) {
    y[i] = x;
    i++;
  }
  print_array(y);
  fclose(fp);
  free(y);
  return 0;
}

void print_array(int* y)
{
  int i;
  for(i=0;i<CHARS;i++) {
    printf("%x ",y[i]);
    if((i+1) % ROWS == 0) {
      printf("\n");
    }
  }
}
