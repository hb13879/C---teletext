#include <stdio.h>
#include <stdlib.h>

#define ROWS 25
#define COLS 40
#define CHARS ROWS*COLS

/*put in dot h file*/
enum colour{black, red, green, yellow, blue, magenta, cyan, white};
enum graphmod{alpnum, contig, seprt};
enum height{sgl, dbl};

typedef enum colour colour;
typedef enum graphmod graphmod;
typedef enum height height;

struct byte {
  int data;
  colour bckgrcol;
  colour frgrcol;
  graphmod graphics;
  height height;
};
typedef struct byte byte;

void print_array(byte* y);
byte* arr_init(void); /*put these in separate file to abstract data type?*/
byte* read_in(byte* y, char* filename);

int main(void)
{
  byte *y;
  y = arr_init();
  y = read_in(y,"test.m7");
  print_array(y);
  free(y);
  return 0;
}

byte* arr_init(void)
{
  int i;
  byte* y;
  y = (byte*) calloc(CHARS,sizeof(byte));
  for(i=0;i<CHARS;i++) {
    (y[i]).data = 0;
    (y[i]).bckgrcol = black;
    (y[i]).frgrcol = white;
    (y[i]).graphics = contig;
    (y[i]).height = sgl;
  }
  return y;
}

byte* read_in(byte* y, char* filename)
{
  FILE* fp;
  int x, i;
  i = 0;
  x = 0;
  fp = fopen(filename,"rb");
  while(fread(&x,1,1,fp)) {
    (y[i]).data = x;
    i++;
  }
  fclose(fp);
  return y;
}

void print_array(byte* y)
{
  int i;
  for(i=0;i<CHARS;i++) {
    printf("%x ",(y[i]).data);
    if((i+1) % ROWS == 0) {
      printf("\n");
    }
  }
}
