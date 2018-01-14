#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SDL_Wrap.h"

#define ROWS 25
#define COLS 40
#define FILESIZE ROWS*COLS
#define BYTE 1
#define ELS 1
#define MIN 128
#define HUE 255
#define FONTFILE "m7fixed.fnt"
#define SPACE ' '
#define MILLISECONDDELAY 20
#define SXH 3 /*Sixel height*/
#define SXW 2 /*Sixel width*/
#define OX FNTWIDTH/SXW
#define OY FNTHEIGHT/SXH
#define ON_ERROR(STR) fprintf(stderr, STR); exit(EXIT_FAILURE)
#define IFNULL(A,B) if(A == NULL) {B;}
/*Nulls?*/
/*ifnull*/

enum shade{black, red, green, yellow, blue, magenta, cyan, white};

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
  colour* background;
  colour* foreground;
};
typedef struct grid grid;

/*Auxiliary functions such as those for intiialising, freeing, reading in and accessing data*/
grid* grid_init(void);
byte** data_init(void);
void free_grid(grid** g);
void free_data(byte*** data);
int get_data(grid* g);
void set_data(grid* g, int a);
heightmd get_heightmd(grid* g);
void read_in(byte** data, char* filename);
void print_array(byte** g);
colour* colour_init(void);
void free_colour(colour** a);
void check_filesize(FILE* fp);

/*Data processing functions*/
void process_data(grid* g);
void check_opcode(grid* g);
void set_graphics(grid* g);
void set_alpha(grid* g);
void set_colour(colour* rgb, shade a);
void set_height(grid* g);
void set_held(grid* g);
void set_background(grid* g);
int set_xy(int xy, int x);
void set_heightmd(grid* g);
void set_newline(grid* g);
int wrap(int a);
void copy_colour(colour* a, colour* b);

/*Functions facilitating rendering data to the screen*/
void process_render(grid* g, SDL_Simplewin *sw);
void render(grid* g, SDL_Simplewin *sw);
void draw_char(grid* g, SDL_Simplewin *sw);
void draw_sixel(grid* g, SDL_Simplewin *sw);
void held_graphic(grid* g,SDL_Simplewin *sw);
void hold_screen(SDL_Simplewin *sw);
char set_char(grid* g);
