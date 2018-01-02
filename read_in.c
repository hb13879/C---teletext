#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "neillsdl2.h"

#define ROWS 25
#define COLS 40
#define CHARS ROWS*COLS
#define MIN 128
#define STRSIZE 100
#define HUE 255

/*put in dot h file*/
enum colour{black, red, green, yellow, blue, magenta, cyan, white};
enum graphmod{alphnum, contig, seprt};
enum height{sgl, dbl};
enum held{on, off};

typedef enum colour colour;
typedef enum graphmod graphmod;
typedef enum height height;
typedef enum held held;

struct byte {
  int data;
  colour bckgrcol;
  colour frgrcol;
  graphmod graphics;
  height height;
  held held;
};
typedef struct byte byte;

void print_array(byte* y);
byte* arr_init(void); /*put these in separate file to abstract data type?*/
byte* read_in(byte* y, char* filename);
void arr_process(byte* y);
void chg_row(byte* y, int i);
void render(byte* y);

int main(void)
{
  byte *y;
  y = arr_init();
  y = read_in(y,"test.m7");
  arr_process(y);
  render(y);
  /*print_array(y);*/
  free(y);
  return 0;
}

void arr_process(byte* y)
{
  int i;
  for(i = 0; i<CHARS; i++) {

/*Change to alphanumeric mode and change foreground colour*/
    if((y[i]).data > 0x80 && (y[i]).data < 0x88) {
      (y[i]).graphics = alphnum;
      (y[i]).frgrcol = (y[i]).data - 0x80;
    }

/*Change to contigious graphics mode and change foreground colour*/
    else if((y[i]).data > 0x90 && (y[i]).data < 0x98) {
      (y[i]).graphics = contig;
      (y[i]).frgrcol = (y[i]).data - 0x90;
    }

/*Change height*/
    else if((y[i]).data >= 0x8C && (y[i]).data <= 0x8D) {
      (y[i]).height = (y[i]).data - 0x8C;
    }

/*Change contigious/separate graphics*/
    else if((y[i]).data >= 0x99 && (y[i]).data <= 0x9A) {
      (y[i]).graphics = (y[i]).data - 0x98;
    }

/*Change background*/
    else if((y[i]).data == 0x9C) {
      (y[i]).bckgrcol = black;
    }
    else if((y[i]).data == 0x9D) {
      (y[i]).bckgrcol = (y[i]).frgrcol;
    }

  /*Held graphics*/
    else if((y[i]).data >= 0x9E && (y[i]).data <= 0x9F) {
      (y[i]).held = (y[i]).data - 0x9E;
    }
    chg_row(y,i);
  }
}

void chg_row(byte* y, int i)
{
  int j;
  for(j = i+1;j % COLS != 0;j++)
  {
    (y[j]).bckgrcol = (y[i]).bckgrcol;
    (y[j]).frgrcol = (y[i]).frgrcol;
    (y[j]).graphics = (y[i]).graphics;
    (y[j]).height = (y[i]).height;
    (y[j]).held = (y[i]).held;
  }
}

byte* arr_init(void)
{
  int i;
  byte* y;
  y = (byte*) calloc(CHARS,sizeof(byte));
  if(y == NULL) {
    fprintf(stderr, "Unable to initialize array\n");
    exit(1);
  }
  for(i=0;i<CHARS;i++) {
    (y[i]).data = 0;
    (y[i]).bckgrcol = black;
    (y[i]).frgrcol = white;
    (y[i]).graphics = contig;
    (y[i]).height = sgl;
    (y[i]).held = off;
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
  if(fp == NULL) {
    fprintf(stderr,"Cannot open file\n");
    exit(1);
  }
  while(fread(&x,1,1,fp)) {
    if(x<MIN) {
      x += MIN;
    }
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
    printf("%x ",(y[i]).frgrcol);
    if((i+1) % COLS == 0) {
      printf("\n");
    }
  }
}


void render(byte* y)
{
  int i;
  i = 0;
  SDL_Simplewin sw;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];

}
