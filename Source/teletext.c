#include "teletext.h"

grid* grid_init(void)
{
  grid* g = (grid*) malloc(sizeof(grid));
  IFNULL(g,ON_ERROR("Grid failed to initialise\n"))
  g->data = data_init();
  g->foreground = colour_init();
  g->background = colour_init();
  g->x = 0;
  g->y = 0;
  set_newline(g); /*this initialises most parameters*/
  g->heldchar = SPACE;
  return g;
}

byte** data_init(void)
{
  int i;
  byte** data = (byte**) calloc(ROWS,sizeof(byte*));
  IFNULL(data,ON_ERROR("Data array failed to initialise\n"))
  for(i=0;i<ROWS;i++) {
    data[i] = (byte*) calloc(COLS, sizeof(byte));
    IFNULL(data,ON_ERROR("Data array failed to initialise\n"))
  }
  return data;
}

void free_grid(grid** g)
{
  if(g == NULL) {
    return;
  }
  else {
    grid* p = *g;
    free_data(p->data);
    free_colour(&((*g)->foreground));
    free_colour(&((*g)->background));
    free(p->data);
    p->data = NULL;
    free(p);
    p = NULL;
  }
}

void free_colour(colour** a)
{
  colour* p = *a;
  free(p);
  p = NULL;
}

void free_data(byte** data)
{
  int i;
  IFNULL(data,ON_ERROR("NULL value passed to free_data\n"))
  for(i=0;i<ROWS;i++) {
    free(data[i]);
    data[i] = NULL;
  }
}

void read_in(byte** data, char* filename)
{
  FILE* fp;
  int i, j;
  fp = fopen(filename,"rb");
  IFNULL(fp,ON_ERROR("Cannot open teletext file\n"))
  IFNULL(data,ON_ERROR("Invalid byte array passed to read in\n"))
  for(i=0;i<ROWS;i++) {
    for(j=0;j<COLS && fread(&((data[i][j]).val),BYTE,ELS,fp);j++) {
      if(data[i][j].val < MIN) {
        data[i][j].val += MIN;
      }
    }
  }
  fclose(fp);
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


void process_render(grid* g, SDL_Simplewin *sw)
{
  IFNULL(g,ON_ERROR("Cannot process null grid"))
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
    set_colour(g->foreground,white);
    set_colour(g->background,black);
    g->dblheight = false;
    g->held = false;
  }
}

void check_opcode(grid* g)
{
  if(get_data(g) == 0x80 || (get_data(g) >= 0x88 && get_data(g) <= 0x8B) \
  || (get_data(g) >= 0x8E && get_data(g) <= 0x90) || get_data(g) == 0x98 \
  || get_data(g) == 0x9B || get_data(g) > 0xFF) {
    fprintf(stderr, "%d,%d\n",g->x,g->y);
    ON_ERROR("Unrecognised opcode at above address\n");
  }
  /*nb no need to check if <0x80 as this was dealt with by read_in function*/
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
    set_colour(g->background,black);
  }
  else if(get_data(g) == 0x9D) {
    copy_colour(g->background, g->foreground);
  }
}

void copy_colour(colour* a, colour* b)
{
  a->r = b->r;
  a->g = b->g;
  a->b = b->b;
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
  int i,j,lit[SXH][SXW] = {{0,1},{2,3},{4,6}};
  rectangle.w = OX;
  rectangle.h = OY;
  for(i=0;i<SXH;i++) {
    for(j=0;j<SXW;j++) {
      /*This bit shifting test tests whether a given portion of the sixel should be lit*/
      /*For example, if the bottom left is lit, then the 4th bit will be a 1*/
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
  /*save this graphics character for if held graphics is switched on*/
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
    /*check whether the byte above is type dbltop. If so then set to dblbtm*/
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
    /* Subtraction to give correct enumeration for colours:*/
    set_colour(g->foreground,get_data(g) - 0x80);
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
    set_colour(g->foreground,get_data(g) - 0x90);
    g->alpha = 0;
  }
}


void print_array(byte** arr)
{
  int i,j;
  for(i=0;i<15;i++) {
    for(j=0;j<COLS;j++) {
      printf("%x ",(arr[i][j]).val);
    }
    printf("\n");
  }
}

void set_colour(colour* rgb, shade a)
{
  rgb->r = 0;
  rgb->g = 0;
  rgb->b = 0;
  if(a == red || a == yellow || a == magenta || a == white) {
    rgb->r = HUE;
  }
  if(a == green || a == yellow || a == cyan || a == white) {
    rgb->g = HUE;
  }
  if(a == blue || a == magenta || a == cyan || a == white) {
    rgb->b = HUE;
  }
}

colour* colour_init(void)
{
  colour* rgb;
  rgb = (colour*) calloc(1,sizeof(colour));
  IFNULL(rgb,ON_ERROR("Colour struct failed to initialise\n"))
  return rgb;
}
