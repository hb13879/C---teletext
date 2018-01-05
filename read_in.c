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
/*magic numbers*/
/*On error*/
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
  byte** data; /*Where the teletext file values are stored*/
  int x;
  int y;
  bool graphics; /*Contigious or Separated*/
  bool alpha; /*Alpanumeric Mode or not - here I assume that if alphanumeric mode is on (as default), this overrides graphics mode. When graphics are selected, it is turned off*/
  bool dblheight;
  bool held; /*Whether hold graphics is on or not */
  int heldchar; /*What the most recent graphic symbol was */
  colour background;
  colour foreground;
};
typedef struct grid grid;

grid* grid_init(void);
byte** data_init(void);
void read_in(byte** data, char* filename);
void print_array(byte** g);
void set_graphics(grid* g);
void set_alpha(grid* g);
colour set_colour(shade a);
void set_height(grid* g);
void set_held(grid* g);
void set_background(grid* g);
int wrap(int a);
void render(grid* g, SDL_Simplewin *sw);
int set_xy(int xy, int x);
void draw_char(grid* g, SDL_Simplewin *sw);
void draw_sixel(grid* g, SDL_Simplewin *sw);
void set_heightmd(grid* g);
void hold_screen(SDL_Simplewin *sw);
void process_render(grid* g, SDL_Simplewin *sw);
void free_grid(grid** g);
void free_data(byte** data);
int get_data(grid* g);
void set_data(grid* g, int a);
heightmd get_heightmd(grid* g);
void process_data(grid* g);
void held_graphic(grid* g,SDL_Simplewin *sw);
char set_char(grid* g);
void check_opcode(grid* g);
void set_newline(grid* g);

int main(int argc, char** argv)
{
  if(argc != 2) {
    fprintf(stderr, "%s\n", "Error - try this");
    exit(1);
  }
  else {
    grid* g;
    SDL_Simplewin sw;
    Neill_SDL_Init(&sw);
    g = grid_init();
    read_in(g->data,argv[1]);
    process_render(g,&sw);
    free_grid(&g);
  }
  return 0;
}

/*Wrappers to facilitate getting and setting data from/in the underlying array*/
int get_data(grid* g)
{
  return g->data[g->y][g->x].val;
}

heightmd get_heightmd(grid* g)
{
  return g->data[g->y][g->x].height;
}


void set_data(grid* g, int a)
{
  g->data[g->y][g->x].val = a;
}

void free_grid(grid** g)
{
  grid* p = *g;
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
  for((g->y)=0;(g->y)<ROWS;(g->y)++) {
    for((g->x)=0;(g->x)<COLS && !sw->finished;(g->x)++) {
      process_data(g);
      render(g, sw);
      Neill_SDL_Events(sw);
    }
  }
  hold_screen(sw);
}

void process_data(grid* g)
{
  check_opcode(g);
  set_newline(g);
  set_graphics(g);
  set_alpha(g);
  set_height(g);
  set_heightmd(g);
  set_held(g);
  set_background(g);
}

void set_newline(grid* g)
{
  if(g->x == 0)
  {
    g->graphics = 0;
    g->alpha = 1;
    g->foreground = set_colour(white);
    g->background = set_colour(black);
    g->dblheight = false;
    g->held = false;
  }
}

void check_opcode(grid* g)
{
  if(get_data(g) == 0x80 || (get_data(g) >= 0x88 && get_data(g) <= 0x8B) \
  || (get_data(g) >= 0x8E && get_data(g) <= 0x90) || get_data(g) == 0x98 \
  || get_data(g) == 0x9B) {
    printf("Error in opcode\n");
    exit(1);
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
  if(get_data(g) == 0x9C) {
    g->background = set_colour(black);
  }
  else if(get_data(g) == 0x9D) {
    g->background = g->foreground;
  }
}

void render(grid* g, SDL_Simplewin *sw)
{
  if(get_data(g) < 0xA0 && g->held) {
    held_graphic(g,sw);
  }
  else if((get_data(g) < 0xA0) || (get_data(g) >= 0xC0 && get_data(g) <= 0xDF) || g->alpha == 1) {
    draw_char(g,sw);
  }
  else {
    draw_sixel(g,sw);
  }
  Neill_SDL_UpdateScreen(sw);
}

void held_graphic(grid* g,SDL_Simplewin *sw)
{
  if(g->heldchar > 0xFF) {
    set_data(g,(g->heldchar)/2);
  }
  else {
    set_data(g,(g->heldchar));
  }
  draw_sixel(g,sw);
}

char set_char(grid* g)
{
  if(get_data(g) >= 0xA0) {
    /*to return correct asci value*/
    return (get_data(g) - MIN);
  }
  else {
    return SPACE;
  }
}

void draw_char(grid* g, SDL_Simplewin *sw)
{
  fntrow fontdata[FNTCHARS][FNTHEIGHT];
  char c = set_char(g);
  Neill_SDL_ReadFont(fontdata, FONTFILE);
  if(get_heightmd(g) == sgl) {
    Neill_SDL_DrawChar(sw,fontdata,c,set_xy(g->x,1),
    set_xy(g->y,0),g->foreground,g->background);
  }
  else if(get_heightmd(g) == dbltop) {
    Neill_SDL_DrawTopHalf(sw,fontdata,c,set_xy(g->x,1),
    set_xy(g->y,0),g->foreground,g->background);
  }
  else {
    Neill_SDL_DrawBottomHalf(sw,fontdata,c,set_xy(g->x,1),
    set_xy(g->y,0),g->foreground,g->background);
  }
}

void draw_sixel(grid* g, SDL_Simplewin *sw)
{
  SDL_Rect rectangle;
  /*lit matrix corresponds to bit position of 1,2,4,8,16,and 64*/
  int i,j,lit[ORIG][DIM] = {{0,1},{2,3},{4,6}};
  rectangle.w = OX;
  rectangle.h = OY;
  for(i=0;i<ORIG;i++) {
    for(j=0;j<DIM;j++) {
      lit[i][j] = (get_data(g) >> lit[i][j]) & 1;
      rectangle.x = set_xy(g->x,1)+j*OX;
      rectangle.y = set_xy(g->y,0)+i*OY;
      if(lit[i][j]) {
        Neill_SDL_SetDrawColour(sw, g->foreground);
        SDL_RenderFillRect(sw->renderer, &rectangle);
      }
      else {
        Neill_SDL_SetDrawColour(sw, g->background);
        SDL_RenderFillRect(sw->renderer, &rectangle);
      }
      if(g->graphics == 1) {
        Neill_SDL_SetDrawColour(sw, g->background);
        SDL_RenderDrawRect(sw->renderer, &rectangle);
      }
    }
  }
  g->heldchar = (get_data(g))*g->graphics;
}

int set_xy(int xy, int x)
{
  if(x) {
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
  if(g->dblheight) {
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
  if(get_data(g) == 0x9E) {
    g->held = 1;
  }
  else if(get_data(g) == 0x9F) {
    g->held = 0;
  }
}

void set_height(grid* g)
{
  if(get_data(g) == 0x8C) {
    g->dblheight = false;
  }
  else if(get_data(g) == 0x8D) {
    g->dblheight = true;
  }
}

void set_alpha(grid* g)
{
  if(get_data(g) > 0x80 && get_data(g) < 0x88) {
    g->alpha = 1;
    /*to give correct enumeration for colours:*/
    g->foreground = set_colour(get_data(g) - 0x80);
  }
}

void set_graphics(grid* g) /*if you turn select a graphics colour or mode, alphanumeric mode turns off*/
{
  if(get_data(g) == 0x99) {
    g->graphics = 0;
    g->alpha = 0;
  }
  else if(get_data(g) == 0x9A) {
    g->graphics = 1;
    g->alpha = 0;
  }
  if(get_data(g) > 0x90 && get_data(g) < 0x98) {
    /*To give correct enumeration for colour*/
    g->foreground = set_colour(get_data(g) - 0x90);
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
  g->held = 0;
  g->dblheight = 0;
  g->alpha = 1;
  g->background = set_colour(black);
  g->foreground = set_colour(white);
  g->heldchar = SPACE;
  return g;
}

byte** data_init(void)
{
  int i;
  byte** data = (byte**) calloc(ROWS,sizeof(byte*));
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
    for(j=0;j<COLS && fread(&((data[i][j]).val),1,1,fp);j++) {
      if(data[i][j].val < MIN) {
        data[i][j].val += MIN;
      }
    }
  }
  fclose(fp);
}

void print_array(byte** arr)
{
  int i,j;
  for(i=0;i<ROWS;i++) {
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
