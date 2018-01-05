#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL_Wrap.h"

#define ROWS 25
#define COLS 40
#define CHARS ROWS*COLS
#define MIN 128
#define HUE 255
#define FONTFILE "m7fixed.fnt"
#define SPACE ' '
#define MILLISECONDDELAY 20
#define ORIG 3
#define DIM 2
#define OX FNTWIDTH/DIM
#define OY FNTHEIGHT/ORIG
/*Nulls?*/
/*put in dot h file*/
enum heightmd{sgl, dbltop, dblbtm};
enum shade{black, red, green, yellow, blue, magenta, cyan, white};

typedef enum heightmd heightmd;
typedef enum shade shade;

struct byte {
  int val;
  heightmd height;
};
typedef struct byte byte;

struct grid {
  byte** data;
  int x;
  int y;
  bool graphics;
  bool alpha;
  bool dblheight;
  bool held;
  int heldchar;
  colour background;
  colour foreground;
};
typedef struct grid grid;

grid* grid_init(void);
byte** data_init(void);
void read_in(byte** data, char* filename);
/*void print_array(byte** g); DELETE*/
void set_graphics(grid* g);
void set_alpha(grid* g);
colour set_colour(shade a);
void set_height(grid* g);
void set_held(grid* g);
void set_background(grid* g);
int wrap(int a);
void render(grid* g, SDL_Simplewin *sw);
int set_coords(int xy, int x);
void draw_char(grid* g, SDL_Simplewin *sw);
void draw_sixel(grid* g, SDL_Simplewin *sw);
void set_heightmd(grid* g);
void hold_screen(SDL_Simplewin *sw);
void process_render(grid* g, SDL_Simplewin *sw);
void free_grid(grid** g);
void free_data(byte** data);

int main(void)
{
  grid* g;
  SDL_Simplewin sw;
  Neill_SDL_Init(&sw);
  g = grid_init();
  read_in(g->data,"test.m7");
  process_render(g,&sw);
  hold_screen(&sw);
  free_grid(&g);
  return 0;
}

void free_grid(grid** g)
{
  grid* p;
  p = *g;
  free_data(p->data);
  free(p->data);
  p->data = NULL;
  free(p);
  p = NULL;
}

void free_data(byte** data)
{
  int i;
  for(i=0;i<ROWS;i++) {
    free(data[i]);
    data[i] = NULL;
  }
}

void process_render(grid* g, SDL_Simplewin *sw)
{
  int i,j;
  for(i=0;i<ROWS;i++) {
    for(j=0;j<COLS && !sw->finished;j++) {
      g->x = j;
      g->y = i;
      set_graphics(g);
      set_alpha(g);
      set_height(g);
      set_heightmd(g);
      set_held(g);
      set_background(g);
      render(g, sw);
      Neill_SDL_Events(sw);
    }
  }
}

void hold_screen(SDL_Simplewin *sw)
{
  do {
    SDL_Delay(MILLISECONDDELAY);
    Neill_SDL_Events(sw);
  } while(!sw->finished);
  atexit(SDL_Quit);
}

void set_background(grid* g)
{
  if(g->data[g->y][g->x].val == 0x9C) {
    g->background = set_colour(black);
  }
  else if(g->data[g->y][g->x].val == 0x9D) {
    g->background = g->foreground;
  }
}

void render(grid* g, SDL_Simplewin *sw)
{
  if((g->data[g->y][g->x].val < 0xA0) || (g->data[g->y][g->x].val >= 0xC0 && g->data[g->y][g->x].val <= 0xDF) || g->alpha == 1) {
    draw_char(g,sw);
  }
  else {
    draw_sixel(g,sw);
  }
}

void draw_char(grid* g, SDL_Simplewin *sw)
{
  if(g->data[g->y][g->x].val < 0xA0 && g->held) {
    if(g->heldchar > 0xFF) {
      g->data[g->y][g->x].val = (g->heldchar)/2;
    }
    else {
      g->data[g->y][g->x].val = g->heldchar;
    }
    draw_sixel(g,sw);
  }
  else {
    char c;
    fntrow fontdata[FNTCHARS][FNTHEIGHT];
    if(g->data[g->y][g->x].val >= 0xA0) {
      c = g->data[g->y][g->x].val - MIN;
    }
    else {
      c = SPACE;
    }
    Neill_SDL_ReadFont(fontdata, FONTFILE);
    if(g->data[g->y][g->x].height == sgl) {
      Neill_SDL_DrawChar(sw,fontdata,c,set_coords(g->x,1),set_coords(g->y,0),g->foreground,g->background);
    }
    else if(g->data[g->y][g->x].height == dbltop) {
      Neill_SDL_DrawTopHalf(sw,fontdata,c,set_coords(g->x,1),set_coords(g->y,0),g->foreground,g->background);
    }
    else if(g->data[g->y][g->x].height == dblbtm) {
      Neill_SDL_DrawBottomHalf(sw,fontdata,c,set_coords(g->x,1),set_coords(g->y,0),g->foreground,g->background);
    }
    Neill_SDL_UpdateScreen(sw);
  }
}

void draw_sixel(grid* g, SDL_Simplewin *sw)
{
  SDL_Rect rectangle;
  int i,j,lit[ORIG][DIM] = {{0,1},{2,3},{4,6}};
  rectangle.w = OX;
  rectangle.h = OY;
  for(i=0;i<ORIG;i++) {
    for(j=0;j<DIM;j++) {
      lit[i][j] = (g->data[g->y][g->x].val >> lit[i][j]) & 1;
      rectangle.x = set_coords(g->x,1)+j*OX;
      rectangle.y = set_coords(g->y,0)+i*OY;
      if(lit[i][j]) {
        Neill_SDL_SetDrawColour(sw, g->foreground);
        SDL_RenderFillRect(sw->renderer, &rectangle);
      }
      if(g->graphics == 1) {
        Neill_SDL_SetDrawColour(sw, g->background);
        SDL_RenderDrawRect(sw->renderer, &rectangle);
      }
    }
  }
  Neill_SDL_UpdateScreen(sw);
  g->heldchar = (g->data[g->y][g->x].val)*g->graphics;
}

int set_coords(int xy, int x)
{
  if(x == 1) {
    return xy*FNTWIDTH;
  }
  else {
    return xy*FNTHEIGHT;
  }
}

int wrap(int a)
{
  if(a < 0) {
    return a + ROWS;
  }
  else {
    return a % ROWS;
  }
}

void set_heightmd(grid* g)
{
  if(g->dblheight == true) {
    if(g->data[wrap((g->y)-1)][g->x].height == dbltop) {
      g->data[g->y][g->x].height = dblbtm;
    }
    else {
      g->data[g->y][g->x].height = dbltop;
    }
  }
}
void set_held(grid* g)
{
  if(g->data[g->y][g->x].val == 0x9E) {
    g->held = 1;
  }
  else if(g->x == 0 || g->data[g->y][g->x].val == 0x9F) {
    g->held = 0;
  }
}

void set_height(grid* g)
{
  if(g->data[g->y][g->x].val == 0x8C) {
    g->dblheight = false;
  }
  else if(g->data[g->y][g->x].val == 0x8D) {
    g->dblheight = true;
  }
}

void set_alpha(grid* g)
{
  if(g->data[g->y][g->x].val > 0x80 && g->data[g->y][g->x].val < 0x88) {
    g->alpha = 1;
    g->foreground = set_colour(g->data[g->y][g->x].val - 0x80);
  }
  else if(g->x == 0) {
    g->alpha = 1;
    g->foreground = set_colour(white);
  }
}

void set_graphics(grid* g) /*if you turn select a graphics colour or mode, alphanumeric mode turns off*/
{
  if(g->x == 0) {
    g->graphics = 0;
    g->alpha = 1;
  }
  if(g->data[g->y][g->x].val == 0x99) { /*if new line or change*/
    g->graphics = 0;
    g->alpha = 0;
  }
  else if(g->data[g->y][g->x].val == 0x9A) {
    g->graphics = 1;
    g->alpha = 0;
  }
  if(g->data[g->y][g->x].val > 0x90 && g->data[g->y][g->x].val < 0x98) {
    g->foreground = set_colour(g->data[g->y][g->x].val - 0x90);
    g->alpha = 0;
  }
}

grid* grid_init(void)
{
  grid* g;
  g = (grid*) malloc(sizeof(grid));
  if(g == NULL) {
    printf("Grid failed to initialise\n");
    exit(1);
  }
  g->data = data_init();
  g->x = 0;
  g->y = 0;
  g->graphics = 0;
  g->alpha = 1;
  g->background = set_colour(black); /*set to black*/
  g->foreground = set_colour(white); /*set to white*/
  g->held = 0;
  g->heldchar = SPACE;
  g->dblheight = false;
  return g;
}

byte** data_init(void)
{
  int i;
  byte** data;
  data = (byte**) calloc(ROWS,sizeof(byte*));
  if(data == NULL) {
    printf("Data array failed to initialise\n");
    exit(1);
  }
  for(i=0;i<ROWS;i++) {
    data[i] = (byte*) calloc(COLS, sizeof(byte));
    if(data[i] == NULL) {
      printf("Data array failed to initialise\n");
      exit(1);
    }
  }
  return data;
}

void read_in(byte** data, char* filename)
{
  FILE* fp;
  int i, j;
  fp = fopen(filename,"rb");
  if(fp == NULL) {
    fprintf(stderr,"Cannot open file\n");
    exit(1);
  }
  for(i=0;i<ROWS;i++) {
    for(j=0;j<COLS && fread(&((data[i][j]).val),1,1,fp);j++);
  }
  fclose(fp);
}

void print_array(byte** arr)
{
  int i,j;
  for(i=18;i<19;i++) {
    for(j=0;j<COLS;j++) {
      printf("%x ",(arr[i][j]).val);
    }
    printf("\n");
  }
}

colour set_colour(shade a)
{
  colour rgb;
  if(a == red || a == yellow || a == magenta || a == white) {
    rgb.r = HUE;
  }
  else {
    rgb.r = 0;
  }
  if(a == green || a == yellow || a == cyan || a == white) {
    rgb.g = HUE;
  }
  else {
    rgb.g = 0;
  }
  if(a == blue || a == magenta || a == cyan || a == white) {
    rgb.b = HUE;
  }
  else {
    rgb.b = 0;
  }
  return rgb;
}
