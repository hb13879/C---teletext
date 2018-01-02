#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL_Wrap.h"

#define ROWS 25
#define COLS 40
#define CHARS ROWS*COLS
#define MIN 128
#define STRSIZE 100
#define HUE 255
#define FONTFILE "m7fixed.fnt"
#define SPACE ' '
#define MILLISECONDDELAY 20000
#define ORIG 3
#define DIM 2
#define OX FNTWIDTH/DIM
#define OY FNTHEIGHT/ORIG

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
void set_colour(shade* rgb, colour a);
void set_coords(unsigned int *x,unsigned int *y, unsigned int i);
void draw_sixels(SDL_Simplewin *sw, unsigned int xx,unsigned int yy, int data, shade rgbf, shade rgbb);

int main(void)
{
  byte *y;
  y = arr_init();
  y = read_in(y,"test.m7");
  arr_process(y);/*
  print_array(y);*/
  render(y);
  free(y);
  printf("%ld\n",FNTWIDTH);
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
    printf("%x ",(y[i]).data);
    if((i+1) % COLS == 0) {
      printf("\n");
    }
  }
}


void render(byte* y)
{
  unsigned int i, xx, yy;
  shade rgbf,rgbb;
  char a;
  SDL_Simplewin sw;
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  Neill_SDL_Init(&sw);
  for(i=0;i<CHARS && !sw.finished;i++) {

    set_coords(&xx, &yy, i);
    set_colour(&rgbf, y[i].frgrcol);
    set_colour(&rgbb, y[i].bckgrcol);
    /*if control code*/
    if(y[i].data < 0xa0) {
      a = SPACE;
      Neill_SDL_ReadFont(fontdata,FONTFILE);
      Neill_SDL_SetDrawColour(&sw, rgbb);
      Neill_SDL_DrawChar(&sw,fontdata,a,xx,yy,rgbf,rgbb);
      Neill_SDL_UpdateScreen(&sw);
    }
    /*if alphanumeric mode*/
    else if(y[i].graphics == alphnum) {
      a = y[i].data - MIN;
      Neill_SDL_ReadFont(fontdata,FONTFILE);
      Neill_SDL_SetDrawColour(&sw, rgbb);
      Neill_SDL_DrawChar(&sw,fontdata,a,xx,yy,rgbf,rgbb);
      Neill_SDL_UpdateScreen(&sw);
    }
    else if(y[i].graphics == contig) {
      draw_sixels(&sw, xx,yy,y[i].data,rgbf,rgbb);
    }
    else {
      a = 'c';
      Neill_SDL_ReadFont(fontdata,FONTFILE);
      Neill_SDL_SetDrawColour(&sw, rgbb);
      Neill_SDL_DrawChar(&sw,fontdata,a,xx,yy,rgbf,rgbb);
      Neill_SDL_UpdateScreen(&sw);
    }
  }
  SDL_Delay(MILLISECONDDELAY);

  if(sw.finished) {
    atexit(SDL_Quit);
  }
}

void draw_sixels(SDL_Simplewin *sw, unsigned int x,unsigned int y, int data, shade rgbf, shade rgbb) /*try without passing colours*/
{
  SDL_Rect rectangle;
  int i,j,lit[ORIG][DIM] = {{1,2},{4,8},{16,64}};
  rectangle.w = OX;
  rectangle.h = OY;
  for(i=0;i<ORIG;i++) {
    for(j=0;j<DIM;j++) {
      lit[i][j] = (data >> lit[i][j]) & 1;
      rectangle.x = x+j*OX;
      rectangle.y = y+i*OY;
      if(lit[i][j]) {
        Neill_SDL_SetDrawColour(sw, rgbf);
        SDL_RenderFillRect(sw->renderer, &rectangle);
      }
      else {
        Neill_SDL_SetDrawColour(sw, rgbb);
        SDL_RenderDrawRect(sw->renderer, &rectangle);
      }
    }
  }
  Neill_SDL_UpdateScreen(sw);
}

void set_coords(unsigned int *x,unsigned int *y, unsigned int i)
{
  *x = (i%COLS)*FNTWIDTH;
  *y = (i/COLS)*FNTHEIGHT;
}

void set_colour(shade* rgb, colour a)
{
  if(a == white || a == red || a == yellow || a == magenta) {
    (*rgb).r = HUE;
  }
  else {
    (*rgb).r = 0;
  }
  if(a == yellow || a == white || a == green || a == cyan) {
    (*rgb).g = HUE;
  }
  else {
    (*rgb).g = 0;
  }
  if(a == cyan || a == blue || a == magenta || a == white) {
    (*rgb).b = HUE;
  }
  else {
    (*rgb).b = 0;
  }
}
