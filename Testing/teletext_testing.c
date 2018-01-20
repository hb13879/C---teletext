#include "teletext.h"
#include <assert.h>

#define NOCHARS 96

void test_init_free(void);
void test_getset_data(void);
void test_setgraphics_function(void);
int test_colour(colour* c, shade a);
void test_setforeground_background(void);
void test_copy_colour(void);
void test_set_height(void);
void test_set_held(void);
void test_set_char(void);
void test_newline(void);

int main(void)
{
  test_init_free();
  test_getset_data();
  test_setgraphics_function();
  test_setforeground_background();
  test_copy_colour();
  test_set_height();
  test_set_held();
  test_set_char();
  test_newline();
  printf("\nAll unit tests passed\n\n");
  return 0;
}

void test_init_free(void)
{
  grid* g;
  grid* h;
  g = grid_init();
  /*check initialisation*/
  assert(g->x == 0);
  assert(g->y == 0);
  assert(g->graphics == 0);
  assert(g->alpha == 1);
  assert(test_colour(g->background,black));
  assert(test_colour(g->foreground,white));
  assert(g->dblheight == false);
  assert(g->held == false);
  assert(g->heldchar == ' ');
  /*test background free functions*/
  h = grid_init();
  free_data(&(h->data));
  assert(h->data == NULL);
  free_colour(&(h->foreground));
  assert(h->foreground == NULL);
  free_colour(&(h->background));
  assert(h->background == NULL);
  /*test main free function*/
  free_grid(&g);
  assert(g == NULL);
  /*Check free functions cope with NULL values*/
  free_data(NULL);
  free_colour(NULL);
  free_grid(NULL);
  printf("Init and free functions OK\n");
}

void test_getset_data(void)
{
  grid* g = grid_init();
  assert(get_data(g) == 0); /*calloc'ed array*/
  assert(get_heightmd(g) == sgl);
  g->x = 5;
  g->y = 7;
  /*test data functions*/
  set_data(g,100);
  assert(get_data(g) == 100);
  assert(g->data[7][5].val == 100);
  /*test height mode function*/
  g->data[7][5].height = dbltop;
  assert(get_heightmd(g) == dbltop);
  printf("Get and set data wrapper functions OK\n");
  free_grid(&g);
}

void test_setgraphics_function(void)
{
  shade i;
  grid* g;
  g = grid_init();
  /*Test each colour has been set correctly individually*/
  for(i=red;i<=white;i++) {
    set_data(g,0x90 + i);
    set_graphics(g);
    assert(test_colour(g->foreground,i));
  }
  assert(g->alpha == 0);
  set_data(g,0x99); /*test contigious graphics mode set correctly*/
  set_graphics(g);
  assert(g->graphics == 0);
  assert(g->alpha == 0);
  set_data(g,0x9A); /*test separated graphics mode set correctly*/
  set_graphics(g);
  assert(g->graphics == 1);
  assert(g->alpha == 0);
  printf("Set graphics function OK\n");
  free_grid(&g);
}

void test_setforeground_background(void)
{
  grid* g = grid_init();
  shade i;
  for(i=red;i<=white;i++) {
    set_data(g,0x80 + i);
    set_alpha(g);
    assert(test_colour(g->foreground,i));
    (g->x)++;
    (g->y)++;
    set_data(g,0x9D);
    set_background(g);
    assert(test_colour(g->background,i));
    (g->x)--;
    (g->y)--;
  }
  printf("Set foreground and background to non black OK\n");
  set_data(g,0x9C);
  assert(test_colour(g->background,black));
  free_grid(&g);
  printf("Set black background OK\n");
  printf("Set_alpha and set_background OK\n");
}

void test_copy_colour(void)
{
  colour* a = colour_init();
  colour* b = colour_init();
  shade i;
  for(i=red;i<=white;i++) {
    set_colour(a,i);
    assert(test_colour(a,i));
    copy_colour(b,a);
    assert(test_colour(b,i));
  }
  free_colour(&a);
  free_colour(&b);
  printf("Set & copy colour functions OK\n");
}

int test_colour(colour* c, shade a)
{
  if(a == red || a == yellow || a == magenta || a == white) {
    if(c->r != HUE) {
      return 0;
    }
  }
  if(a == green || a == yellow || a == cyan || a == white) {
    if(c->g != HUE) {
      return 0;
    }
  }
  if(a == blue || a == magenta || a == cyan || a == white) {
    if(c->b != HUE) {
      return 0;
    }
  }
  return 1;
}

void test_set_height(void)
{
  grid* g = grid_init();
  set_data(g,0x8C);
  set_height(g);
  assert(g->dblheight == false);
  set_heightmd(g);
  assert(get_heightmd(g) == sgl);
  set_data(g,0x8D);
  set_height(g);
  assert(g->dblheight == true);
  set_heightmd(g);
  assert(get_heightmd(g) == dbltop);
  (g->y)+=1;
  set_data(g,0x8D);
  set_height(g);
  set_heightmd(g);
  assert(get_heightmd(g) == dblbtm);
  free_grid(&g);
  printf("Set_height and Set_heightmd functions OK\n");
}

void test_set_held(void)
{
  grid* g = grid_init();
  set_data(g,0x9E);
  set_held(g);
  assert(g->held == 1);
  set_data(g,0x9F);
  set_held(g);
  assert(g->held == 0);
  free_grid(&g);
  printf("Held graphics OK\n");
}

void test_set_char(void)
{
  grid* g = grid_init();
  int i;
  char c, d;
  for(i=0;i<NOCHARS;i++) {
    set_data(g,0xA0 + i);
    d = get_data(g) - MIN;
    c = set_char(g);
    assert(c == d);
  }
  free_grid(&g);
  printf("Set_char OK\n");
}

void test_newline(void)
{
  grid* g = grid_init();
  (g->y)++;
  set_newline(g);
  assert(g->graphics == 0);
  assert(g->alpha == 1);
  assert(test_colour(g->background,black));
  assert(test_colour(g->foreground,white));
  assert(g->dblheight == false);
  assert(g->held == false);
  assert(g->heldchar == ' ');
  free_grid(&g);
  printf("Newline operation OK\n");
}
